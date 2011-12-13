#include <SFGUI/SFGUI.hpp>

#include <SFML/Graphics.hpp>
#include <sstream>

// Create our button smart pointer.
sfg::ComboBox::Ptr combo_box;
sfg::Label::Ptr sel_label( sfg::Label::Create( L"Please select an item!" ) );

void OnComboSelect();
void OnAddItemClick();

int main() {
	// Create the main SFML window
	sf::RenderWindow app_window( sf::VideoMode( 800, 600 ), "SFGUI Combo Box Example", sf::Style::Titlebar | sf::Style::Close );

	// Create our main SFGUI window
	sfg::Window::Ptr window;
	window = sfg::Window::Create();
	window->SetTitle( "Title" );

	// Create the combo box itself.
	combo_box = sfg::ComboBox::Create();

	// Set the entries of the combo box.
	combo_box->AppendItem( "Bar" );
	combo_box->PrependItem( "Foo" );

	sfg::Button::Ptr button( sfg::Button::Create( L"Add item" ) );

	sfg::Box::Ptr hbox( sfg::Box::Create( sfg::Box::HORIZONTAL, 5 ) );
	hbox->Pack( combo_box );
	hbox->Pack( button, false );

	sfg::Box::Ptr vbox( sfg::Box::Create( sfg::Box::VERTICAL, 5 ) );
	vbox->Pack( hbox, false );
	vbox->Pack( sel_label, true );

	// Add the combo box to the window
	window->Add( vbox );

	// So that our combo box has a meaningful purpose (besides just looking
	// awesome :P) we need to tell it to connect to a callback of our choosing to
	// notify us when it is clicked.
	combo_box->OnSelect.Connect( &OnComboSelect );

	button->OnClick.Connect( &OnAddItemClick );

	// If attempting to connect to a class method you need to provide
	// a pointer to it as the second parameter after the function address.

	// Start the game loop
	while ( app_window.IsOpened() ) {
		// Process events
		sf::Event event;

		while ( app_window.PollEvent( event ) ) {
			// Handle events
			window->HandleEvent( event );

			// Close window : exit
			if ( event.Type == sf::Event::Closed ) {
				app_window.Close();
			}
		}

		// Clear screen
		app_window.Clear();

		// Draw the window
		window->Expose( app_window );

		// Update the window
		app_window.Display();
	}

	return EXIT_SUCCESS;
}

void OnComboSelect() {
	std::stringstream sstr;

	sstr << "Item " << combo_box->GetSelectedItem() << " selected with text \"" << static_cast<std::string>( combo_box->GetSelectedText() ) << "\"";
	sel_label->SetText( sstr.str() );
}

void OnAddItemClick() {
	static int counter( 0 );

	std::stringstream sstr;
	sstr << "Item " << counter;
	combo_box->AppendItem( sstr.str() );

	++counter;
}
