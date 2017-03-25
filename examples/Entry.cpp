// Always include the necessary header files.
// Including SFGUI/Widgets.hpp includes everything
// you can possibly need automatically.
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>

int main() {
	// Create the main SFML window
	sf::RenderWindow app_window( sf::VideoMode( 800, 600 ), "SFGUI Entry Example", sf::Style::Titlebar | sf::Style::Close );

	// Create an SFGUI. This is required before doing anything with SFGUI.
	sfg::SFGUI sfgui;

	// We have to do this because we don't use SFML to draw.
	app_window.resetGLStates();

	// Create our main SFGUI window
	auto window = sfg::Window::Create();
	window->SetTitle( "Title" );

	// Create our box.
	auto box = sfg::Box::Create( sfg::Box::Orientation::HORIZONTAL );

	// Create a button.
	auto button = sfg::Button::Create();
	button->SetLabel( "Set" );

	// Create a label.
	auto label = sfg::Label::Create();
	label->SetText( "no text yet" );

	// Create our entry widget itself.
	auto entry = sfg::Entry::Create();

	// Connect the button.
	button->GetSignal( sfg::Widget::OnLeftClick ).Connect( [&label, &entry] {
		// When the button is clicked set the contents of the label
		// to the contents of the entry widget.
		label->SetText( entry->GetText() );
	} );

	// Until now all widgets only expanded to fit the text inside of it.
	// This is not the case with the entry widget which can be empty
	// but still has to have a reasonable size.
	// To disable the automatic sizing of widgets in general you can
	// use the SetRequisition() method. it takes an sf::Vector as it's
	// parameter. Depending on which side you want to have a minimum size,
	// you set the corresponding value in the vector.
	// Here we chose to set the minimum x size of the widget to 80.
	entry->SetRequisition( sf::Vector2f( 80.f, 0.f ) );

	// Setting sizing back to automatic is as easy as setting
	// x and y sizes to 0.

	// Pack into box
	box->Pack( entry );
	box->Pack( button );
	box->Pack( label );

	// Set box spacing
	box->SetSpacing( 5.f );

	// Add our box to the window
	window->Add( box );

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
				return EXIT_SUCCESS;
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
