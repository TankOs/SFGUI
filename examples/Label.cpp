#include <SFML/Graphics.hpp>

// Always include the necessary header files.
// These mostly have the form SFGUI/<Widget name>.hpp
#include <SFGUI/Window.hpp>
#include <SFGUI/Label.hpp>

int main() {
	// Create the main SFML window
	sf::RenderWindow app_window( sf::VideoMode( 800, 600 ), "SFGUI Label Example" );

	// Create our main SFGUI window
	sfg::Window::Ptr window;
	window = sfg::Window::Create();
	window->SetTitle( "Title" );

	// Create our label smart pointer.
	sfg::Label::Ptr label;

	// Create the label itself.
	label = sfg::Label::Create();

	// Set the text of the label.
	label->SetText( "Hello World!" );

	// Add the label to the window.

	// Windows are a subclass of the Bin widget type. Bins are only
	// allowed to contain one child widget so adding more than 1 widget
	// to a window will result in a warning and no effect. For adding more
	// widgets to a window refer to the Box example later on. For a full
	// widget hierarchy refer to the documentation.
	window->Add( label );

	// Setting the border width of a window keeps it's child widget away
	// from the edges of the window.
	window->SetBorderWidth( 10.f );

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
