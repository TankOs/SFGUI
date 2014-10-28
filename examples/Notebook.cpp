// Always include the necessary header files.
// Including SFGUI/Widgets.hpp includes everything
// you can possibly need automatically.
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>

int main() {
	// Create the main SFML window
	sf::RenderWindow app_window( sf::VideoMode( 800, 600 ), "SFGUI Notebook Example", sf::Style::Titlebar | sf::Style::Close );

	// We have to do this because we don't use SFML to draw.
	app_window.resetGLStates();

	// Create an SFGUI. This is required before doing anything with SFGUI.
	sfg::SFGUI sfgui;

	// Create our main SFGUI window
	auto window = sfg::Window::Create();
	window->SetTitle( "Title" );

	// Create the notebook.
	auto notebook = sfg::Notebook::Create();

	// Create a couple of buttons to populate the notebook.
	auto button1 = sfg::Button::Create("Hello");
	auto button2 = sfg::Button::Create("World");

	// Add new pages to the notebook with respective tab labels
	// containing solely the buttons as their children.
	notebook->AppendPage( button1, sfg::Label::Create( "Page 1" ) );
	notebook->AppendPage( button2, sfg::Label::Create( "Page 2" ) );
	notebook->AppendPage( sfg::Label::Create(), sfg::Label::Create( "Page 3" ) );
	notebook->AppendPage( sfg::Label::Create(), sfg::Label::Create( "Page 4" ) );
	notebook->AppendPage( sfg::Label::Create(), sfg::Label::Create( "Page 5" ) );
	notebook->AppendPage( sfg::Label::Create(), sfg::Label::Create( "Page 6" ) );
	notebook->AppendPage( sfg::Label::Create(), sfg::Label::Create( "Page 7" ) );
	notebook->AppendPage( sfg::Label::Create(), sfg::Label::Create( "Page 8" ) );

	notebook->SetScrollable( true );
	notebook->SetRequisition( sf::Vector2f( 200.f, 0.f ) );

	// Add the notebook to the window.
	window->Add( notebook );

	sf::Clock clock;

	// Update an initial time to construct the GUI before drawing begins.
	// This makes sure that there are no frames in which no GUI is visible.
	window->Update( 0.f );

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

		// Update the GUI every 5ms
		if( clock.getElapsedTime().asMicroseconds() >= 5000 ) {
			// Update() takes the elapsed time in seconds.
			window->Update( static_cast<float>( clock.getElapsedTime().asMicroseconds() ) / 1000000.f );

			clock.restart();
		}

		// Clear screen
		app_window.clear();

		// Draw the GUI
		sfgui.Display( app_window );

		// Update the window
		app_window.display();
	}

	return EXIT_SUCCESS;
}
