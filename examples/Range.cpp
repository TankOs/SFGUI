// Always include the necessary header files.
// Including SFGUI/Widgets.hpp includes everything
// you can possibly need automatically.
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>
#include <sstream>

class RangeExample {
	public:
		void AdjustmentChange();

		void Run();

	private:
		// Create an SFGUI. This is required before doing anything with SFGUI.
		sfg::SFGUI m_sfgui;

		// Create our label smart pointer.
		sfg::Label::Ptr m_label;

		// Create our adjustment smart pointer.
		sfg::Adjustment::Ptr m_adjustment;
};

void RangeExample::AdjustmentChange() {
	std::stringstream sstr;
	sstr << m_adjustment->GetValue();
	m_label->SetText( sstr.str() );
}

void RangeExample::Run() {
	// Create the main SFML window
	sf::RenderWindow app_window( sf::VideoMode( 800, 600 ), "SFGUI Range Example", sf::Style::Titlebar | sf::Style::Close );

	// We have to do this because we don't use SFML to draw.
	app_window.resetGLStates();

	// Create our main SFGUI window
	auto window = sfg::Window::Create();
	window->SetTitle( "Title" );

	// Create our box.
	auto box = sfg::Box::Create( sfg::Box::Orientation::HORIZONTAL );

	// Create a label.
	m_label = sfg::Label::Create();
	m_label->SetText( "20" );

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
	m_adjustment = scrollbar->GetAdjustment();
	scale->SetAdjustment( m_adjustment );

	// Tune the adjustment parameters.
	m_adjustment->SetLower( 20.f );
	m_adjustment->SetUpper( 100.f );

	// How much it should change when clicked on the stepper.
	m_adjustment->SetMinorStep( 3.f );

	// How much it should change when clicked on the trough.
	m_adjustment->SetMajorStep( 12.f );

	// CAUTION:
	// Normally you would only set the page size for scrollbar adjustments.
	// For demonstration purposes we do this for our scale widget too.
	// If page size isn't 0 a scale widget won't be able to be set to it's
	// maximum value. This is in fact also true for scrollbars, however
	// because they are used to scroll the page size must be subtracted from
	// the maximum.
	m_adjustment->SetPageSize( 20.f );

	// Additionally you can connect to the OnChange signal of an adjustment
	// to get notified when any of it's parameters are changed.
	m_adjustment->GetSignal( sfg::Adjustment::OnChange ).Connect( std::bind( &RangeExample::AdjustmentChange, this ) );

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
	box->Pack( m_label );

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
		m_sfgui.Display( app_window );

		// Update the window
		app_window.display();
	}
}

int main() {
	RangeExample example;
	example.Run();

	return EXIT_SUCCESS;
}
