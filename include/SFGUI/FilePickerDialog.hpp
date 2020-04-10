#pragma once

#include <SFGUI/Config.hpp>

#if defined(SFGUI_BOOST_FILESYSTEM_SUPPORT)

#include <boost/filesystem.hpp>

#include <SFGUI/Box.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/ComboBox.hpp>
#include <SFGUI/Entry.hpp>
#include <SFGUI/ListBox.hpp>
#include <SFGUI/Window.hpp>

namespace sfg {

class SFGUI_API FilePickerDialog : public Window {
    public:
        typedef std::shared_ptr<FilePickerDialog> Ptr; //!< Shared pointer.
		typedef std::shared_ptr<const FilePickerDialog> PtrConst; //!< Shared pointer.

        static Ptr Create( boost::filesystem::path initial_path );

        sf::String GetSelectedPath() const;

        static Signal::SignalID OnCancel;
        static Signal::SignalID OnOk;

    protected:
        FilePickerDialog( boost::filesystem::path initial_path );

    private:
        sfg::Box::Ptr m_main_box;
            sfg::Box::Ptr m_panel_box;
                sfg::ListBox::Ptr m_locations_listbox;

                sfg::Box::Ptr m_directory_box;
                    sfg::Box::Ptr m_directory_entry_box;
                        sfg::Button::Ptr m_new_directory_button;
                        sfg::Entry::Ptr m_current_directory_entry;
                    sfg::ListBox::Ptr m_directory_paths_listbox;

                    sfg::Box::Ptr m_filename_box;
                        sfg::Entry::Ptr m_filename_entry;

            sfg::Box::Ptr m_buttons_box;
                sfg::Button::Ptr m_ok_button;
                sfg::Button::Ptr m_cancel_button;

        boost::filesystem::path m_current_path;

        bool m_show_hidden_files;

        void UpdateLocationsPaths();
        void UpdateCurrentDirectoryPath();
        void UpdateDirectoryPaths();
        void UpdateOkButtonState();

        void CancelDialog();
        void OkDialog();

        void OnLocationsListBoxSelectionChanged();
        void OnCurrentDirectoryEntryTextChanged();
        void OnPathsListBoxSelectionChanged();
        void OnFilenameEntryTextChanged();
};

}

#endif
