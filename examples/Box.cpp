// Always include the necessary header files.
// Including SFGUI/Widgets.hpp includes everything
// you can possibly need automatically.
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>

int main() {
	// Create the main SFML window
	sf::RenderWindow app_window( sf::VideoMode( 800, 600 ), "SFGUI Box Example", sf::Style::Titlebar | sf::Style::Close );

	// We have to do this because we don't use SFML to draw.
	app_window.resetGLStates();

	// Create an SFGUI. This is required before doing anything with SFGUI.
	sfg::SFGUI sfgui;

	// Create our main SFGUI window
	auto window = sfg::Window::Create();
	window->SetTitle( "Title" );

	// Since only being able to add one widget to a window is very limiting
	// there are Box widgets. They are a subclass of the Container class and
	// can contain an unlimited amount of child widgets. Not only that, they
	// also have the ability to lay out your widgets nicely.

	// Create the box.
	// For layout purposes we must specify in what direction new widgets
	// should be added, horizontally or vertically.
	auto box = sfg::Box::Create( sfg::Box::Orientation::HORIZONTAL );

	auto button1 = sfg::Button::Create();
	auto button2 = sfg::Button::Create();
	auto label = sfg::Label::Create();

	button1->SetLabel( "Foo" );
	button2->SetLabel( "Bar" );
	label->SetText( "Baz" );

	// To add our widgets to the box we use the Pack() method instead of the
	// Add() method. This makes sure the widgets are added and layed out
	// properly in the box.
	box->Pack( button1 );
	box->Pack( label );
	box->Pack( button2 );

	// Just as with the window we can set the spacing between widgets
	// withing a box.
	box->SetSpacing( 5.f );

	// Finally we add our box to the window as it's only child.
	// Notice that we don't have to add the children of a box to it's parent
	// Because all children and grandchildren and .... are automatically
	// considered descendents of the parent.
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
