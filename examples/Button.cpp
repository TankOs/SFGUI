#include <SFGUI/SFGUI.hpp>
#include <SFML/Graphics.hpp>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Create the label label pointer globally to reach it from OnButtonClick().
sfg::Label::Ptr g_label;

void OnButtonClick() {
	g_label->SetText( "Hello SFGUI, pleased to meet you!" );
}

int main() {
	// Create SFML's window.
	sf::RenderWindow render_window( sf::VideoMode( SCREEN_WIDTH, SCREEN_HEIGHT ), "Hello world!" );

	// Install SFGUI guard that prevents applications from crashing at exit. You
	// only need to create one object, but make sure to do it!
	sfg::SFGUI guard;

	// Create the label.
	g_label = sfg::Label::Create( "Hello world!" );

	// Create a simple button and connect the click signal.
	sfg::Button::Ptr button( sfg::Button::Create( "Greet SFGUI!" ) );
	button->OnClick.Connect( &OnButtonClick );

	// Create a vertical box layouter with 5 pixels spacing and add the label
	// and button to it.
	sfg::Box::Ptr box( sfg::Box::Create( sfg::Box::VERTICAL, 5.0f ) );
	box->Pack( g_label );
	box->Pack( button, false );

	// Create a window and add the box layouter to it. Also set the window's title.
	sfg::Window::Ptr window( sfg::Window::Create() );
	window->SetTitle( "Hello world!" );
	window->Add( box );

	// Create a desktop and add the window to it.
	sfg::Desktop desktop;
	desktop.Add( window );

	// We're not using SFML to render anything in this program, so reset OpenGL
	// states. Otherwise we wouldn't see anything.
	render_window.resetGLStates();

	// Main loop!
	sf::Event event;
	sf::Clock clock;

	while( render_window.isOpen() ) {
		// Event processing.
		while( render_window.pollEvent( event ) ) {
			desktop.HandleEvent( event );

			// If window is about to be closed, leave program.
			if( event.type == sf::Event::Closed ) {
				render_window.close();
			}
		}

		// Update SFGUI with elapsed seconds since last call.
		desktop.Update( clock.restart().asSeconds() );

		// Rendering.
		render_window.clear();
		sfg::Renderer::Get().Display( render_window );
		render_window.display();
	}

	return 0;
}
