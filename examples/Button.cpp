#include <SFML/Graphics.hpp>

// Always include the necessary header files.
// These mostly have the form SFGUI/<Widget name>.hpp
#include <SFGUI/Window.hpp>
#include <SFGUI/Button.hpp>

// Create our button smart pointer.
sfg::Button::Ptr button;

void ButtonClick();

int main() {
	// Create the main SFML window
	sf::RenderWindow app_window( sf::VideoMode( 800, 600 ), "SFGUI Button Example" );

	// Create our main SFGUI window
	sfg::Window::Ptr window;
	window = sfg::Window::Create();
	window->SetTitle( "Title" );

	// Create the button itself.
	button = sfg::Button::Create();

	// Set the label of the button.
	button->SetLabel( "Foo" );

	// Add the button to the window
	window->Add( button );

	// So that our button has a meaningful purpose
	// (besides just looking awesome :P) we need to tell it to connect
	// to a callback of our choosing to notify us when it is clicked.
	button->OnClick.Connect( &ButtonClick );

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

void ButtonClick() {
	// When the button is clicked it's label should change.
	button->SetLabel( "Bar" );
}
