// Always include the necessary header files.
// Including SFGUI/Widgets.hpp includes everything
// you can possibly need automatically.
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>

int main() {
	// Create the main SFML window
	sf::RenderWindow app_window( sf::VideoMode( 800, 600 ), "SFGUI Layout Example", sf::Style::Titlebar | sf::Style::Close );

	// We have to do this because we don't use SFML to draw.
	app_window.resetGLStates();

	// Create an SFGUI. This is required before doing anything with SFGUI.
	sfg::SFGUI sfgui;

	// Create our main SFGUI window
	auto window = sfg::Window::Create();
	window->SetTitle( "Resize me!" );

	// Create a box with 20 pixels spacing.
	auto box = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 20.f );

	// Create some buttons.
	auto button_aligned = sfg::Button::Create( "Aligned 0.8f right and bottom" );
	auto button_fixed = sfg::Button::Create( "Fixed at (200, 50)" );

	// Create a fixed.
	auto fixed = sfg::Fixed::Create();

	// Add button_fixed to the fixed at (200, 50).
	fixed->Put( button_fixed, sf::Vector2f( 200.f, 50.f ) );

	// Add our fixed to the box and set not to expand.
	box->Pack( fixed, false, true );

	// Create a separator.
	auto separator = sfg::Separator::Create( sfg::Separator::Orientation::HORIZONTAL );

	// Add separator to box and set not to expand.
	box->Pack( separator, false, true );

	// Create an alignment.
	auto alignment = sfg::Alignment::Create();

	// Because we want to align our button horizontally, we need
	// to place our alignment in a horizontal box set to expand and fill.
	auto alignment_box = sfg::Box::Create( sfg::Box::Orientation::HORIZONTAL );
	alignment_box->Pack( alignment, true, true );

	// Add button_aligned to the alignment.
	alignment->Add( button_aligned );

	// Set scaling parameters.
	// Scaling sets how much of the total space in the alignment
	// the child gets to fill up. 1.0f for x and y would mean the
	// child gets to use the whole space. Setting it to 0.0f for
	// x and y makes the child get as little space as possible.
	alignment->SetScale( sf::Vector2f( .0f, .0f ) );

	// Set alignment parameters.
	// Alignment aligns the child widget within the alignment's area.
	// 0.0f for x aligns to the left, 1.0f for x aligns to the right.
	// 0.0f for y aligns to the top, 1.0f for y aligns to the bottom.
	alignment->SetAlignment( sf::Vector2f( .8f, .8f ) );

	// Create our frame.
	auto frame = sfg::Frame::Create( "Frame Title" );

	// We can align the frame title just like an alignment.
	frame->SetAlignment( sf::Vector2f( .8f, .5f ) );

	// Add our alignment box into the frame.
	frame->Add( alignment_box );

	// Add our frame to the box.
	box->Pack( frame, true, true );

	// Add the box to the window.
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
				app_window.close();
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
