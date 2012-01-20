#include <SFML/Graphics.hpp>

// Always include the necessary header files.
// Including SFGUI/SFGUI.hpp includes everything
// you can possibly need automatically.
#include <SFGUI/SFGUI.hpp>

int main() {
	// Create the main SFML window
	sf::RenderWindow app_window( sf::VideoMode( 800, 600 ), "SFGUI Notebook Example", sf::Style::Titlebar | sf::Style::Close );

	// Create our main SFGUI window
	sfg::Window::Ptr window;
	window = sfg::Window::Create();
	window->SetTitle( "Title" );

	// Create our notebook smart pointer.
	sfg::Notebook::Ptr notebook;

	// Create the notebook itself.
	notebook = sfg::Notebook::Create();

	// Create a couple of buttons to populate the notebook.
	sfg::Button::Ptr button1( sfg::Button::Create("Hello") );
	sfg::Button::Ptr button2( sfg::Button::Create("World") );

	// Add new pages to the notebook with respective tab labels
	// containing solely the buttons as their children.
	notebook->AppendPage( button1, sfg::Label::Create( "Page 1" ) );
	notebook->AppendPage( button2, sfg::Label::Create( "Page 2" ) );

	// Add the notebook to the window.
	window->Add( notebook );

	// Start the game loop
	while ( app_window.IsOpen() ) {
		// Process events
		sf::Event event;

		while ( app_window.PollEvent( event ) ) {
			// Handle events
			window->HandleEvent( event );

			// Close window : exit
			if ( event.Type == sf::Event::Closed ) {
				app_window.Close();
			}
		}

		// Update the GUI, note that you shouldn't normally
		// pass 0 seconds to the update method.
		window->Update( 0.f );

		// Clear screen
		app_window.Clear();

		// Draw the GUI
		sfg::Renderer::Get().Display( app_window );

		// Update the window
		app_window.Display();
	}

	return EXIT_SUCCESS;
}
