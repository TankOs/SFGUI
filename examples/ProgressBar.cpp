// Always include the necessary header files.
// Including SFGUI/Widgets.hpp includes everything
// you can possibly need automatically.
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>

int main() {
	// Create the main SFML window
	sf::RenderWindow app_window( sf::VideoMode( 800, 600 ), "SFGUI Progress Bar Example", sf::Style::Titlebar | sf::Style::Close );

	// We have to do this because we don't use SFML to draw.
	app_window.resetGLStates();

	// Create an SFGUI. This is required before doing anything with SFGUI.
	sfg::SFGUI sfgui;

	// Create our main SFGUI window
	auto window = sfg::Window::Create();
	window->SetTitle( "Title" );

	// Create our progress bar
	auto progressbar = sfg::ProgressBar::Create();

	// Set how big the progress bar should be
	progressbar->SetRequisition( sf::Vector2f( 200.f, 40.f ) );

	// Create a button and connect the click signal.
	auto button = sfg::Button::Create( "Set Random Value" );

	button->GetSignal( sfg::Widget::OnLeftClick ).Connect( [&progressbar] {
		// Generate random value for progress bar percent done
		auto random_percentage = static_cast<float>( rand() ) / static_cast<float>( RAND_MAX );
		progressbar->SetFraction(random_percentage);
	} );

	// Create a horizontal box layouter and add widgets to it.
	auto box = sfg::Box::Create( sfg::Box::Orientation::HORIZONTAL, 5.0f );
	box->Pack( progressbar );
	box->Pack( button, false );

	// Add the box to the window.
	window->Add( box );

	// Our clock
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
				return 0;
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

	return 0;
}
