#include <SFML/Graphics.hpp>

// Always include the necessary header files.
// Including SFGUI/SFGUI.hpp includes everything
// you can possibly need automatically.
#include <SFGUI/SFGUI.hpp>

int main() {
	// Create the main SFML window
	sf::RenderWindow app_window( sf::VideoMode( 800, 600 ), "SFGUI Canvas Example", sf::Style::Titlebar | sf::Style::Close );

	// We have to do this because we don't use SFML to draw.
	app_window.resetGLStates();

	// Create an SFGUI. This is required before doing anything with SFGUI.
	sfg::SFGUI sfgui;

	// Create our OpenGL canvas window
	auto opengl_window = sfg::Window::Create();
	opengl_window->SetTitle( "OpenGL canvas" );
	opengl_window->SetPosition( sf::Vector2f( 100.f, 100.f ) );

	// Create our SFML canvas window
	auto sfml_window = sfg::Window::Create();
	sfml_window->SetTitle( "SFML canvas" );
	sfml_window->SetPosition( sf::Vector2f( 400.f, 100.f ) );

	// Create the Canvases.
	// Passing true in to Create() tells SFGUI
	// to create a depth buffer for the canvas.
	// This might be needed for your OpenGL rendering.
	// Specifying nothing defaults to no depth buffer.
	auto opengl_canvas = sfg::Canvas::Create( true );
	auto sfml_canvas = sfg::Canvas::Create();

	// Add the Canvases to the windows.
	opengl_window->Add( opengl_canvas );
	sfml_window->Add( sfml_canvas );

	// Create an sf::Sprite for demonstration purposes.
	sf::Texture texture;
	texture.loadFromFile( "data/sfgui.png" );
	sf::Sprite sprite;
	sprite.setTexture( texture );

	// Because Canvases provide a virtual surface to draw
	// on much like a ScrolledWindow, specifying their
	// minimum requisition is necessary.
	opengl_canvas->SetRequisition( sf::Vector2f( 200.f, 150.f ) );
	sfml_canvas->SetRequisition( sf::Vector2f( texture.getSize() ) );

	// Create a desktop to contain our Windows.
	sfg::Desktop desktop;
	desktop.Add( opengl_window );
	desktop.Add( sfml_window );

	sf::Clock clock;

	// Start the game loop
	while ( app_window.isOpen() ) {
		// Process events
		sf::Event event;

		while ( app_window.pollEvent( event ) ) {
			// Handle events
			desktop.HandleEvent( event );

			// Close window : exit
			if ( event.type == sf::Event::Closed ) {
				app_window.close();
			}
		}

		// Update the GUI, note that you shouldn't normally
		// pass 0 seconds to the update method.
		desktop.Update( 0.f );

		// Clear screen
		app_window.clear();

		// We bind the Canvases and draw whatever we want to them.
		// This must occur BEFORE the GUI is displayed.
		// You must not forget to set whatever context was active
		// before to active again after you unbind a Canvas.
		// Clear() takes an RGBA color and a bool which specifies
		// if you want to clear the depth buffer as well. In this
		// case, we do.

		// First the Canvas for OpenGL rendering.
		opengl_canvas->Bind();
		opengl_canvas->Clear( sf::Color( 0, 0, 0, 0 ), true );

		// Do some OpenGL drawing. If you don't intend
		// on drawing with OpenGL just ignore this section.
		glEnable( GL_DEPTH_TEST );
		glDepthMask( GL_TRUE );
		glDisable( GL_TEXTURE_2D );

		glMatrixMode( GL_MODELVIEW );
		glPushMatrix();
		glLoadIdentity();

		glTranslatef( 0.f, 0.f, -2.f );

		glRotatef( clock.getElapsedTime().asSeconds() * 30.f, 0.f, 0.f, 1.f );

		glDisable( GL_TEXTURE_2D );

		glMatrixMode( GL_PROJECTION );
		glPushMatrix();
		glLoadIdentity();

		glViewport( 0, 0, static_cast<int>( opengl_canvas->GetAllocation().width ), static_cast<int>( opengl_canvas->GetAllocation().height ) );

		gluPerspective( 90.f, opengl_canvas->GetAllocation().width / opengl_canvas->GetAllocation().height, 1.f, 20.f );

		glBegin( GL_QUADS );
		glVertex2s( -1, 1 );
		glVertex2s( -1, -1 );
		glVertex2s( 1, -1 );
		glVertex2s( 1, 1 );
		glEnd();

		glPopMatrix();
		glMatrixMode( GL_MODELVIEW );
		glPopMatrix();

		glViewport( 0, 0, app_window.getSize().x, app_window.getSize().y );

		glEnable( GL_TEXTURE_2D );
		glDisable( GL_DEPTH_TEST );

		opengl_canvas->Display();
		opengl_canvas->Unbind();

		// Then the Canvas for SFML rendering.
		sfml_canvas->Bind();
		sfml_canvas->Clear( sf::Color( 0, 0, 0, 0 ) );

		// Draw the SFML Sprite.
		sfml_canvas->Draw( sprite );

		sfml_canvas->Display();
		sfml_canvas->Unbind();

		// This is important.
		app_window.setActive( true );

		// Draw the GUI
		sfgui.Display( app_window );

		// Update the window
		app_window.display();
	}

	return EXIT_SUCCESS;
}
