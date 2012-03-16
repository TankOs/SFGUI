#include <SFML/Graphics.hpp>

// Always include the necessary header files.
// Including SFGUI/SFGUI.hpp includes everything
// you can possibly need automatically.
#include <SFGUI/SFGUI.hpp>

// Create our entry smart pointer.
sfg::Entry::Ptr entry;

// Create our label smart pointer.
sfg::Label::Ptr label;

void ButtonClick();

int main() {
	// Create the main SFML window
	sf::RenderWindow app_window( sf::VideoMode( 800, 600 ), "SFGUI Entry Example", sf::Style::Titlebar | sf::Style::Close );

	// Construct our SFML guard
	// See http://sfgui.sfml-dev.de/forum/topic52-crash-on-close.html for more info.
	sfg::SFGUI sfgui;

	// Create our main SFGUI window
	sfg::Window::Ptr window;
	window = sfg::Window::Create();
	window->SetTitle( "Title" );

	// Create our box.
	sfg::Box::Ptr box = sfg::Box::Create( sfg::Box::HORIZONTAL );

	// Create a button.
	sfg::Button::Ptr button = sfg::Button::Create();
	button->SetLabel( "Set" );

	// Connect the button.
	button->OnClick.Connect( &ButtonClick );

	// Create a label.
	label = sfg::Label::Create();
	label->SetText( "no text yet" );

	// Create our entry widget itself.
	entry = sfg::Entry::Create();

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
		sfg::Renderer::Get().Display( app_window );

		// Update the window
		app_window.display();
	}

	// If you have any global or static widgets,
	// you need to reset their pointers before your
	// application exits.
	entry.reset();
	label.reset();

	return EXIT_SUCCESS;
}

void ButtonClick() {
	// When the button is clicked set the contents of the label
	// to the contents of the entry widget.
	label->SetText( entry->GetText() );
}
