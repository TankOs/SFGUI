// Always include the necessary header files.
// Including SFGUI/Widgets.hpp includes everything
// you can possibly need automatically.
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>

int main() {
	// Create the main SFML window
	sf::RenderWindow app_window( sf::VideoMode( 800, 600 ), "SFGUI Buttons Example", sf::Style::Titlebar | sf::Style::Close );

	// We have to do this because we don't use SFML to draw.
	app_window.resetGLStates();

	// Create an SFGUI. This is required before doing anything with SFGUI.
	sfg::SFGUI sfgui;

	// Create our main SFGUI window
	auto window = sfg::Window::Create();
	window->SetTitle( "Title" );

	// Create a Box to contain all our fun buttons ;)
	auto box = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5.f );

	// Create the Button itself.
	auto button = sfg::Button::Create( "Click me" );

	// Add the Button to the Box
	box->Pack( button );

	// So that our Button has a meaningful purpose
	// (besides just looking awesome :P) we need to tell it to connect
	// to a callback of our choosing to notify us when it is clicked.
	button->GetSignal( sfg::Widget::OnLeftClick ).Connect( [&button] {
		// When the Button is clicked it's label should change.
		button->SetLabel( "I was clicked" );
	} );

	// If attempting to connect to a class method you need to provide
	// a pointer to it as the second parameter after the function address.
	// Refer to the Signals example for more information.

	// Create the ToggleButton itself.
	auto toggle_button = sfg::ToggleButton::Create( "Toggle me" );

	// Connect the OnToggle signal to our handler.
	toggle_button->GetSignal( sfg::ToggleButton::OnToggle ).Connect( [&toggle_button, &window] {
		// When the ToggleButton is active hide the window's titlebar.
		if( toggle_button->IsActive() ) {
			window->SetStyle( window->GetStyle() ^ sfg::Window::TITLEBAR );
		}
		else {
			window->SetStyle( window->GetStyle() | sfg::Window::TITLEBAR );
		}
	} );

	// Add the ToggleButton to the Box
	box->Pack( toggle_button );

	// Create the CheckButton itself.
	auto check_button = sfg::CheckButton::Create( "Check me" );

	// Since a CheckButton is also a ToggleButton we can use
	// ToggleButton signals to handle events for CheckButtons.
	check_button->GetSignal( sfg::ToggleButton::OnToggle ).Connect( [&check_button, &window] {
		// When the CheckButton is active hide the window's background.
		if( check_button->IsActive() ) {
			window->SetStyle( window->GetStyle() ^ sfg::Window::BACKGROUND );
		}
		else {
			window->SetStyle( window->GetStyle() | sfg::Window::BACKGROUND );
		}
	} );

	// Add the CheckButton to the Box
	box->Pack( check_button );

	// Just to keep things tidy ;)
	box->Pack( sfg::Separator::Create() );

	// Create our RadioButtons.
	// RadioButtons each have a group they belong to. If not specified,
	// a new group is created by default for each RadioButton. You can
	// then use RadioButton::SetGroup() to set the group of a RadioButton
	// after you create them. If you already know which buttons will belong
	// to the same group you can just pass the group of the first button
	// to the following buttons when you construct them as we have done here.
	auto radio_button1 = sfg::RadioButton::Create( "Either this" );
	auto radio_button2 = sfg::RadioButton::Create( "Or this", radio_button1->GetGroup() );
	auto radio_button3 = sfg::RadioButton::Create( "Or maybe even this", radio_button1->GetGroup() );

	// Set the third RadioButton to be the active one.
	// By default none of the RadioButtons are active at start.
	radio_button3->SetActive( true );

	auto button_select = [&radio_button1, &radio_button2, &radio_button3, &window] {
		// Depending on which RadioButton is active
		// we set the window title accordingly.
		if( radio_button1->IsActive() ) {
			window->SetTitle( "First button selected" );
		}
		else if( radio_button2->IsActive() ) {
			window->SetTitle( "Second button selected" );
		}
		else if( radio_button3->IsActive() ) {
			window->SetTitle( "Third button selected" );
		}
	};

	// Here we use the same handler for all three RadioButtons.
	// RadioButtons are CheckButtons and therefore also ToggleButtons,
	// hence we can use ToggleButton signals with RadioButtons as well.
	radio_button1->GetSignal( sfg::ToggleButton::OnToggle ).Connect( button_select );
	radio_button2->GetSignal( sfg::ToggleButton::OnToggle ).Connect( button_select );
	radio_button3->GetSignal( sfg::ToggleButton::OnToggle ).Connect( button_select );

	// Add the RadioButtons to the Box
	box->Pack( radio_button1 );
	box->Pack( radio_button2 );
	box->Pack( radio_button3 );

	// Finally add the Box to the window.
	window->Add( box );

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
