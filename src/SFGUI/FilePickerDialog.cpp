#include <SFGUI/FilePickerDialog.hpp>

#if defined(SFGUI_BOOST_FILESYSTEM_SUPPORT)

#include <boost/predef.h>
#if BOOST_OS_LINUX
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif

namespace fs = boost::filesystem;

namespace sfg {

FilePickerDialog::Ptr FilePickerDialog::Create( boost::filesystem::path initial_path ) {
    auto ptr = Ptr( new FilePickerDialog( initial_path ) );
    ptr->Add(ptr->m_main_box);

    ptr->m_main_box->PackEnd(ptr->m_panel_box, true, true);
    ptr->m_main_box->PackEnd(ptr->m_buttons_box, false, true);

    ptr->m_panel_box->PackEnd(ptr->m_locations_listbox, false, true);
    ptr->m_panel_box->PackEnd(ptr->m_directory_box, true, true);

    ptr->m_directory_box->PackEnd(ptr->m_directory_entry_box, false, true);
    ptr->m_directory_box->PackEnd(ptr->m_directory_paths_listbox, true, true);
    ptr->m_directory_box->PackEnd(ptr->m_filename_box, false, true);

    ptr->m_directory_entry_box->PackEnd(ptr->m_current_directory_entry, true, true);
    ptr->m_directory_entry_box->PackEnd(ptr->m_new_directory_button, false, true);

    ptr->m_filename_box->PackEnd(ptr->m_filename_entry, true, true);

    ptr->m_buttons_box->PackEnd(ptr->m_ok_button, true, true);
    ptr->m_buttons_box->PackEnd(ptr->m_cancel_button, false, true);

    ptr->RequestResize();
    ptr->m_locations_listbox->RequestResize();
    ptr->m_new_directory_button->RequestResize();
    ptr->m_current_directory_entry->RequestResize();
    ptr->m_directory_paths_listbox->RequestResize();
    ptr->m_filename_entry->RequestResize();
    ptr->m_ok_button->RequestResize();
    ptr->m_cancel_button->RequestResize();

    return ptr;
}

FilePickerDialog::FilePickerDialog( boost::filesystem::path initial_path ) :
    Window( Window::Style::TOPLEVEL ),
    m_main_box( sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5.f ) ),
    m_panel_box( sfg::Box::Create( sfg::Box::Orientation::HORIZONTAL, 5.f ) ),
    m_locations_listbox( sfg::ListBox::Create( ) ),
    m_directory_box( sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5.f ) ),
    m_directory_entry_box( sfg::Box::Create( sfg::Box::Orientation::HORIZONTAL, 5.f ) ),
    m_new_directory_button( sfg::Button::Create( "+" ) ),
    m_current_directory_entry( sfg::Entry::Create( ) ),
    m_directory_paths_listbox( sfg::ListBox::Create( ) ),
    m_filename_box( sfg::Box::Create( sfg::Box::Orientation::HORIZONTAL, 5.f ) ),
    m_filename_entry( sfg::Entry::Create( ) ),
    m_buttons_box( sfg::Box::Create( sfg::Box::Orientation::HORIZONTAL, 5.f ) ),
    m_ok_button( sfg::Button::Create( "Ok" ) ),
    m_cancel_button( sfg::Button::Create( "Cancel" ) ),
    m_current_path( initial_path ),
    m_show_hidden_files( false )
{
    UpdateLocationsPaths();
    UpdateCurrentDirectoryPath();
    UpdateDirectoryPaths();
    UpdateOkButtonState();

    m_directory_paths_listbox->GetSignal( sfg::ListBox::OnSelect ).Connect( std::bind( &FilePickerDialog::OnPathsListBoxSelectionChanged, this ) );
    m_filename_entry->GetSignal( sfg::Entry::OnTextChanged ).Connect( std::bind( &FilePickerDialog::OnFilenameEntryTextChanged, this ) );
}

void FilePickerDialog::UpdateLocationsPaths() {
#if BOOST_OS_WINDOWS
    //Test all possible drives and add the existing ones into the location list
    for( wchar_t driveLetter = L'A'; driveLetter != L'Z'; driveLetter++ ) {
        if( fs::exists( fs::path( driveLetter + L':' + L'\\' ) ) ) {
            m_locations_listbox->AppendItem( driveLetter + L':' + L'\\' );
        }
    }
#elif BOOST_OS_LINUX
    //Add some common locations
    m_locations_listbox->AppendItem( "/" );

    //Add the current user's home
    const char *homedir;
    if ((homedir = getenv("HOME")) == NULL) {
        homedir = getpwuid(getuid())->pw_dir;
    }

    m_locations_listbox->AppendItem( std::string(homedir) );

#endif
}

void FilePickerDialog::UpdateCurrentDirectoryPath() {
    m_current_directory_entry->SetText( m_current_path.wstring() );
}

void FilePickerDialog::UpdateDirectoryPaths() {
    m_directory_paths_listbox->Clear();
    m_filename_entry->SetText( "" ); //Clear the selected filename

    try {
        if( fs::exists( m_current_path ) && fs::is_directory( m_current_path ) ) {
            m_directory_paths_listbox->AppendItem("..");

            std::vector<fs::path> subdirectories;
            std::vector<fs::path> subfiles;

            //Iterate all files and directories
            for( fs::directory_entry& e : fs::directory_iterator( m_current_path ) ) {
                if( fs::is_directory( e.path() ) ) {
                    if( e.path().filename().wstring()[0] != L'.' || m_show_hidden_files ) {
                        subdirectories.push_back( e.path().filename() );
                    }
                }
                else if( fs::is_regular_file( e.path() ) ) {
                    if( e.path().filename().wstring()[0] != L'.' || m_show_hidden_files ) {
                        subfiles.push_back( e.path().filename() );
                    }
                }
            }

            std::sort( subdirectories.begin(), subdirectories.end() );
            std::sort( subfiles.begin(), subfiles.end() );

            for( fs::path& path : subdirectories ) {
                m_directory_paths_listbox->AppendItem(path.filename().wstring());
            }
            for( fs::path& path : subfiles ) {
                m_directory_paths_listbox->AppendItem(path.filename().wstring());
            }
        }
    }
    catch( const fs::filesystem_error& ex ) {

    }
}

void FilePickerDialog::UpdateOkButtonState() {
    if( m_filename_entry->GetText().getSize() > 0 &&
        fs::exists( fs::absolute( m_filename_entry->GetText().toWideString(), m_current_path ) ) &&
        fs::is_regular_file( fs::absolute( m_filename_entry->GetText().toWideString(), m_current_path ) ) ) {
        m_ok_button->SetState( sfg::Widget::State::NORMAL );
    }
    else {
        m_ok_button->SetState( sfg::Widget::State::INSENSITIVE );
    }
}

void FilePickerDialog::OnPathsListBoxSelectionChanged() {
    if(m_directory_paths_listbox->GetSelectedItemsCount() > 0) {
        fs::path selected_path = m_directory_paths_listbox->GetSelectedItemText(0).toWideString();

        if( fs::is_directory( fs::absolute(selected_path, m_current_path) ) ) {
            fs::path new_path = fs::canonical(selected_path, m_current_path);
            m_current_path = new_path;

            UpdateCurrentDirectoryPath();
            UpdateDirectoryPaths();
            UpdateOkButtonState();
        }
        else if( fs::is_regular_file( fs::absolute(selected_path, m_current_path) ) ) {
            m_filename_entry->SetText( selected_path.wstring() );
            UpdateOkButtonState();
        }
    }
}

void FilePickerDialog::OnFilenameEntryTextChanged() {
    m_directory_paths_listbox->ClearSelection();

    //Try to select the corresponding entry in the folder subpaths list.
    for( sfg::ListBox::IndexType i = 0; i < m_directory_paths_listbox->GetItemsCount(); ++i ) {
        if( m_filename_entry->GetText() == m_directory_paths_listbox->GetItemText( i ) ) {
            m_directory_paths_listbox->SetSelection( i );
        }
    }

    UpdateOkButtonState();
}

}

#endif
