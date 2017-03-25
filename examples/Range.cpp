// Always include the necessary header files.
// Including SFGUI/Widgets.hpp includes everything
// you can possibly need automatically.
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>
#include <sstream>

int main() {
	// Create the main SFML window
	sf::RenderWindow app_window( sf::VideoMode( 800, 600 ), "SFGUI Range Example", sf::Style::Titlebar | sf::Style::Close );

	// Create an SFGUI. This is required before doing anything with SFGUI.
	sfg::SFGUI sfgui;

	// We have to do this because we don't use SFML to draw.
	app_window.resetGLStates();

	// Create our main SFGUI window
	auto window = sfg::Window::Create();
	window->SetTitle( "Title" );

	// Create our box.
	auto box = sfg::Box::Create( sfg::Box::Orientation::HORIZONTAL );

	// Create a label.
	auto label = sfg::Label::Create();
	label->SetText( "20" );

	// Scale and Scrollbar widgets are subclasses of the Range class.
	// They have a common data representation object known as an
	// Adjustment. The Adjustment that each range widget is bound to
	// determines it's current state (where the slider is, what max
	// and min values are, how much to scroll per step etc.). Because
	// range widgets share this common data object they can also be
	// linked together by a common Adjustment instance. An Adjustment
	// is created automatically for you when you create a range widget.

	// Create the scale.
	// We want a horizontal scale.
	auto scale = sfg::Scale::Create( sfg::Scale::Orientation::HORIZONTAL );

	// Create the scrollbar.
	// We want a vertical scrollbar.
	auto scrollbar = sfg::Scrollbar::Create( sfg::Scrollbar::Orientation::VERTICAL );

	// We can link both widgets together by their adjustments.
	auto adjustment = scrollbar->GetAdjustment();
	scale->SetAdjustment( adjustment );

	// Tune the adjustment parameters.
	adjustment->SetLower( 20.f );
	adjustment->SetUpper( 100.f );

	// How much it should change when clicked on the stepper.
	adjustment->SetMinorStep( 3.f );

	// How much it should change when clicked on the trough.
	adjustment->SetMajorStep( 12.f );

	// CAUTION:
	// Normally you would only set the page size for scrollbar adjustments.
	// For demonstration purposes we do this for our scale widget too.
	// If page size isn't 0 a scale widget won't be able to be set to it's
	// maximum value. This is in fact also true for scrollbars, however
	// because they are used to scroll the page size must be subtracted from
	// the maximum.
	adjustment->SetPageSize( 20.f );

	// Additionally you can connect to the OnChange signal of an adjustment
	// to get notified when any of it's parameters are changed.
	adjustment->GetSignal( sfg::Adjustment::OnChange ).Connect( [&adjustment, &label] {
		std::stringstream sstr;
		sstr << adjustment->GetValue();
		label->SetText( sstr.str() );
	} );

	// Just as with the entry widget we set custom requisitions for our
	// range widgets to make sure they don't look strange.
	scale->SetRequisition( sf::Vector2f( 80.f, 20.f ) );
	scrollbar->SetRequisition( sf::Vector2f( 0.f, 80.f ) );

	// To keep our scale's slider from expanding too much we use another box
	// set to verticle orientation.
	auto scalebox = sfg::Box::Create( sfg::Box::Orientation::VERTICAL );
	scalebox->Pack( scale, false, false );

	// Pack into box
	box->Pack( scalebox );
	box->Pack( scrollbar );
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
