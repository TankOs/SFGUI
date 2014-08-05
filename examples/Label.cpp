// Always include the necessary header files.
// Including SFGUI/Widgets.hpp includes everything
// you can possibly need automatically.
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>

int main() {
	// Create the main SFML window
	sf::RenderWindow app_window( sf::VideoMode( 800, 600 ), "SFGUI Label Example", sf::Style::Titlebar | sf::Style::Close );

	// We have to do this because we don't use SFML to draw.
	app_window.resetGLStates();

	// Create an SFGUI. This is required before doing anything with SFGUI.
	sfg::SFGUI sfgui;

	// Create our main SFGUI window
	auto window = sfg::Window::Create();
	window->SetTitle( "Title" );

	// Create the label.
	auto label = sfg::Label::Create();

	// Set the text of the label.
	label->SetText( "Hello World!\nAnother Line" );

	// Add the label to the window.

	// Windows are a subclass of the Bin widget type. Bins are only
	// allowed to contain one child widget so adding more than 1 widget
	// to a window will result in a warning and no effect. For adding more
	// widgets to a window refer to the Box example later on. For a full
	// widget hierarchy refer to the documentation.
	window->Add( label );

	// Start the game loop
	while ( app_window.isOpen() ) {
		// Process events
		sf::Event event;

		while ( app_window.pollEvent( event ) ) {
			// Handle events
			window->HandleEvent( event );

			// Close window : exit
			if ( event.type == sf::Event::Closed ) {
				app_window.close();
			}
		}

		// Update the GUI, note that you shouldn't normally
		// pass 0 seconds to the update method.
		window->Update( 0.f );

		// Clear screen
		app_window.clear();

		// Draw the GUI
		sfgui.Display( app_window );

		// Update the window
		app_window.display();
	}

	return EXIT_SUCCESS;
}
