#include <SFML/Graphics.hpp>

// Always include the necessary header files.
// These mostly have the form SFGUI/<Widget name>.hpp
#include <SFGUI/Window.hpp>
#include <SFGUI/ComboBox.hpp>

// Create our button smart pointer.
sfg::ComboBox::Ptr combo_box;

void ComboSelect();

int main() {
	// Create the main SFML window
	sf::RenderWindow app_window( sf::VideoMode( 800, 600 ), "SFGUI Combo Box Example" );

	// Create our main SFGUI window
	sfg::Window::Ptr window;
	window = sfg::Window::Create();
	window->SetTitle( "Title" );

	// Create the combo box itself.
	combo_box = sfg::ComboBox::Create();

	// Set the entries of the combo box.
	combo_box->AppendText( "Bar" );
	combo_box->PrependText( "Foo" );

	// Add the combo box to the window
	window->Add( combo_box );
	window->SetBorderWidth( 10.f );

	// So that our combo box has a meaningful purpose
	// (besides just looking awesome :P) we need to tell it to connect
	// to a callback of our choosing to notify us when it is clicked.
	combo_box->OnSelect.Connect( &ComboSelect );

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

void ComboSelect() {
	// If an item is selected, that item should change its name.
	int active = combo_box->GetActive();
	if ( combo_box->GetActiveText() == "Foo" ) {
		combo_box->ChangeText( active, "Bar" );
	}
	else {
		combo_box->ChangeText( active, "Foo" );
	}
}
