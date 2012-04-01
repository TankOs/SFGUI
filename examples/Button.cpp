#include <SFML/Graphics.hpp>

// Always include the necessary header files.
// Including SFGUI/SFGUI.hpp includes everything
// you can possibly need automatically.
#include <SFGUI/SFGUI.hpp>

// Create our button smart pointer.
sfg::Button::Ptr button;

void ButtonClick();

int main() {
	// Create the main SFML window
	sf::RenderWindow app_window( sf::VideoMode( 800, 600 ), "SFGUI Button Example", sf::Style::Titlebar | sf::Style::Close );

	// We have to do this because we don't use SFML to draw.
	app_window.resetGLStates();

	// Construct our SFML guard
	// See http://sfgui.sfml-dev.de/forum/topic52-crash-on-close.html for more info.
	sfg::SFGUI sfgui;

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
		sfg::Renderer::Get().Display( app_window );

		// Update the window
		app_window.display();
	}

	// If you have any global or static widgets,
	// you need to reset their pointers before your
	// application exits.
	button.reset();

	return EXIT_SUCCESS;
}

void ButtonClick() {
	// When the button is clicked it's label should change.
	button->SetLabel( "Bar" );
}
