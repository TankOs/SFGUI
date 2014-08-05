// Always include the necessary header files.
// Including SFGUI/Widgets.hpp includes everything
// you can possibly need automatically.
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>

int main() {
	// Create the main SFML window
	sf::RenderWindow app_window( sf::VideoMode( 800, 600 ), "SFGUI Window Example", sf::Style::Titlebar | sf::Style::Close );

	// We have to do this because we don't use SFML to draw.
	app_window.resetGLStates();

	// Create an SFGUI. This is required before doing anything with SFGUI.
	sfg::SFGUI sfgui;

	// Create our main SFGUI window

	// Almost everything in SFGUI is handled through smart pointers
	// for automatic resource management purposes. You create them
	// and they will automatically be destroyed when the time comes.

	// Creation of widgets is always done with it's Create() method
	// which will return a smart pointer owning the new widget.
	auto window = sfg::Window::Create();

	// Here we can set the window's title bar text.
	window->SetTitle( "A really really really really long title" );

	// For more things to set around the window refer to the
	// API documentation.

	// Start the game loop
	while ( app_window.isOpen() ) {
		// Process events
		sf::Event event;

		while ( app_window.pollEvent( event ) ) {
			// Every frame we have to send SFML events to the window
			// to enable user interactivity. Without doing this your
			// GUI is nothing but a big colorful picture ;)
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

		// After drawing the rest of your game, you have to let the GUI
		// render itself. If you don't do this you will never be able
		// to see it ;)
		sfgui.Display( app_window );

		// NOTICE
		// Because the window doesn't have any children it will shrink to
		// it's minimum size of (0,0) resulting in you not seeing anything
		// except the title bar text ;) Don't worry, in the Label example
		// you'll get to see more.

		// Update the window
		app_window.display();
	}

	return EXIT_SUCCESS;
}
