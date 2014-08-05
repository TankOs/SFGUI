// Always include the necessary header files.
// Including SFGUI/Widgets.hpp includes everything
// you can possibly need automatically.
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>

class ButtonsExample {
	public:
		void ButtonClick();

		void ButtonToggle();

		void ButtonCheck();

		void ButtonSelect();

		void Run();

	private:
		// Create an SFGUI. This is required before doing anything with SFGUI.
		sfg::SFGUI m_sfgui;

		// Create our window smart pointer.
		sfg::Window::Ptr m_window;

		// Create our Button smart pointer.
		sfg::Button::Ptr m_button;

		// Create our ToggleButton smart pointer.
		sfg::ToggleButton::Ptr m_toggle_button;

		// Create our CheckButton smart pointer.
		sfg::CheckButton::Ptr m_check_button;

		// Create our RadioButton smart pointers.
		sfg::RadioButton::Ptr m_radio_button1;
		sfg::RadioButton::Ptr m_radio_button2;
		sfg::RadioButton::Ptr m_radio_button3;
};

void ButtonsExample::ButtonClick() {
	// When the Button is clicked it's label should change.
	m_button->SetLabel( "I was clicked" );
}

void ButtonsExample::ButtonToggle() {
	// When the ToggleButton is active hide the window's titlebar.
	if( m_toggle_button->IsActive() ) {
		m_window->SetStyle( m_window->GetStyle() ^ sfg::Window::TITLEBAR );
	}
	else {
		m_window->SetStyle( m_window->GetStyle() | sfg::Window::TITLEBAR );
	}
}

void ButtonsExample::ButtonCheck() {
	// When the CheckButton is active hide the window's background.
	if( m_check_button->IsActive() ) {
		m_window->SetStyle( m_window->GetStyle() ^ sfg::Window::BACKGROUND );
	}
	else {
		m_window->SetStyle( m_window->GetStyle() | sfg::Window::BACKGROUND );
	}
}

void ButtonsExample::ButtonSelect() {
	// Depending on which RadioButton is active
	// we set the window title accordingly.
	if( m_radio_button1->IsActive() ) {
		m_window->SetTitle( "First button selected" );
	}
	else if( m_radio_button2->IsActive() ) {
		m_window->SetTitle( "Second button selected" );
	}
	else if( m_radio_button3->IsActive() ) {
		m_window->SetTitle( "Third button selected" );
	}
}

void ButtonsExample::Run() {
	// Create the main SFML window
	sf::RenderWindow app_window( sf::VideoMode( 800, 600 ), "SFGUI Buttons Example", sf::Style::Titlebar | sf::Style::Close );

	// We have to do this because we don't use SFML to draw.
	app_window.resetGLStates();

	// Create our main SFGUI window
	m_window = sfg::Window::Create();
	m_window->SetTitle( "Title" );

	// Create a Box to contain all our fun buttons ;)
	auto box = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5.f );

	// Create the Button itself.
	m_button = sfg::Button::Create( "Click me" );

	// Add the Button to the Box
	box->Pack( m_button );

	// So that our Button has a meaningful purpose
	// (besides just looking awesome :P) we need to tell it to connect
	// to a callback of our choosing to notify us when it is clicked.
	m_button->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &ButtonsExample::ButtonClick, this ) );

	// If attempting to connect to a class method you need to provide
	// a pointer to it as the second parameter after the function address.
	// Refer to the Signals example for more information.

	// Create the ToggleButton itself.
	m_toggle_button = sfg::ToggleButton::Create( "Toggle me" );

	// Connect the OnToggle signal to our handler.
	m_toggle_button->GetSignal( sfg::ToggleButton::OnToggle ).Connect( std::bind( &ButtonsExample::ButtonToggle, this ) );

	// Add the ToggleButton to the Box
	box->Pack( m_toggle_button );

	// Create the CheckButton itself.
	m_check_button = sfg::CheckButton::Create( "Check me" );

	// Since a CheckButton is also a ToggleButton we can use
	// ToggleButton signals to handle events for CheckButtons.
	m_check_button->GetSignal( sfg::ToggleButton::OnToggle ).Connect( std::bind( &ButtonsExample::ButtonCheck, this ) );

	// Add the CheckButton to the Box
	box->Pack( m_check_button );

	// Just to keep things tidy ;)
	box->Pack( sfg::Separator::Create() );

	// Create our RadioButtons.
	// RadioButtons each have a group they belong to. If not specified,
	// a new group is created by default for each RadioButton. You can
	// then use RadioButton::SetGroup() to set the group of a RadioButton
	// after you create them. If you already know which buttons will belong
	// to the same group you can just pass the group of the first button
	// to the following buttons when you construct them as we have done here.
	m_radio_button1 = sfg::RadioButton::Create( "Either this" );
	m_radio_button2 = sfg::RadioButton::Create( "Or this", m_radio_button1->GetGroup() );
	m_radio_button3 = sfg::RadioButton::Create( "Or maybe even this", m_radio_button1->GetGroup() );

	// Set the third RadioButton to be the active one.
	// By default none of the RadioButtons are active at start.
	m_radio_button3->SetActive( true );

	// Here we use the same handler for all three RadioButtons.
	// RadioButtons are CheckButtons and therefore also ToggleButtons,
	// hence we can use ToggleButton signals with RadioButtons as well.
	m_radio_button1->GetSignal( sfg::ToggleButton::OnToggle ).Connect( std::bind( &ButtonsExample::ButtonSelect, this ) );
	m_radio_button2->GetSignal( sfg::ToggleButton::OnToggle ).Connect( std::bind( &ButtonsExample::ButtonSelect, this ) );
	m_radio_button3->GetSignal( sfg::ToggleButton::OnToggle ).Connect( std::bind( &ButtonsExample::ButtonSelect, this ) );

	// Add the RadioButtons to the Box
	box->Pack( m_radio_button1 );
	box->Pack( m_radio_button2 );
	box->Pack( m_radio_button3 );

	// Finally add the Box to the window.
	m_window->Add( box );

	// Start the game loop
	while ( app_window.isOpen() ) {
		// Process events
		sf::Event event;

		while ( app_window.pollEvent( event ) ) {
			// Handle events
			m_window->HandleEvent( event );

			// Close window : exit
			if ( event.type == sf::Event::Closed ) {
				app_window.close();
			}
		}

		// Update the GUI, note that you shouldn't normally
		// pass 0 seconds to the update method.
		m_window->Update( 0.f );

		// Clear screen
		app_window.clear();

		// Draw the GUI
		m_sfgui.Display( app_window );

		// Update the window
		app_window.display();
	}
}

int main() {
	ButtonsExample example;
	example.Run();

	return EXIT_SUCCESS;
}
