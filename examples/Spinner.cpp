// Always include the necessary header files.
// Including SFGUI/Widgets.hpp includes everything
// you can possibly need automatically.
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>

class SpinnerExample {
	public:
		// Our button click handler.
		void OnButtonClick();

		void Run();

	private:
		// Create an SFGUI. This is required before doing anything with SFGUI.
		sfg::SFGUI m_sfgui;

		// Create the spinner pointer here to reach it from OnButtonClick().
		sfg::Spinner::Ptr m_spinner;
};

void SpinnerExample::OnButtonClick() {
	// If the spinner is spinning...
	if( m_spinner->Started() ) {
		// ... stop the spinner.
		m_spinner->Stop();

		return;
	}

	// ... otherwise start it.
	m_spinner->Start();
}

void SpinnerExample::Run() {
	// Create the main SFML window
	sf::RenderWindow app_window( sf::VideoMode( 800, 600 ), "SFGUI Spinner Example", sf::Style::Titlebar | sf::Style::Close );

	// We have to do this because we don't use SFML to draw.
	app_window.resetGLStates();

	// Create our main SFGUI window
	auto window = sfg::Window::Create();
	window->SetTitle( "Title" );

	// Create our spinner
	m_spinner = sfg::Spinner::Create();

	// Set how big the spinner should be
	m_spinner->SetRequisition( sf::Vector2f( 40.f, 40.f ) );

	// Create a button and connect the click signal.
	auto button = sfg::Button::Create( "Toggle" );
	button->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &SpinnerExample::OnButtonClick, this ) );

	// Create a horizontal box layouter and add widgets to it.
	auto box = sfg::Box::Create( sfg::Box::Orientation::HORIZONTAL, 5.0f );
	box->Pack( m_spinner );
	box->Pack( button, false );

	// Add the box to the window.
	window->Add( box );

	// Our clock to make the spinner spin ;)
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
	SpinnerExample example;
	example.Run();

	return 0;
}
