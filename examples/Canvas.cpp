// Always include the necessary header files.
// Including SFGUI/Widgets.hpp includes everything
// you can possibly need automatically.
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <cmath>

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
	opengl_window->SetPosition( sf::Vector2f( 50.f, 50.f ) );

	// Create our SFML canvas window
	auto sfml_window = sfg::Window::Create();
	sfml_window->SetTitle( "SFML canvas" );
	sfml_window->SetPosition( sf::Vector2f( 300.f, 50.f ) );

	// Create our SFML scrollable canvas window
	auto sfml_scrollable_window = sfg::Window::Create( sfg::Window::Style::TITLEBAR | sfg::Window::Style::BACKGROUND );
	sfml_scrollable_window->SetTitle( "SFML scrollable canvas" );
	sfml_scrollable_window->SetPosition( sf::Vector2f( 300.f, 200.f ) );

	// Create the Canvases.
	// Passing true in to Create() tells SFGUI
	// to create a depth buffer for the canvas.
	// This might be needed for your OpenGL rendering.
	// Specifying nothing defaults to no depth buffer.
	auto opengl_canvas = sfg::Canvas::Create( true );
	auto sfml_canvas = sfg::Canvas::Create();
	auto sfml_scrollable_canvas = sfg::Canvas::Create();

	// Create a pair of scrollbars.
	auto horizontal_scrollbar = sfg::Scrollbar::Create( sfg::Scrollbar::Orientation::HORIZONTAL );
	auto vertical_scrollbar = sfg::Scrollbar::Create( sfg::Scrollbar::Orientation::VERTICAL );

	// Create a table to put the scrollbars and scrollable canvas in.
	auto table = sfg::Table::Create();
	table->Attach( sfml_scrollable_canvas, sf::Rect<sf::Uint32>( 0, 0, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL | sfg::Table::EXPAND );
	table->Attach( vertical_scrollbar, sf::Rect<sf::Uint32>( 1, 0, 1, 1 ), 0, sfg::Table::FILL );
	table->Attach( horizontal_scrollbar, sf::Rect<sf::Uint32>( 0, 1, 1, 1 ), sfg::Table::FILL, 0 );

	// Add the Canvases to the windows.
	opengl_window->Add( opengl_canvas );
	sfml_window->Add( sfml_canvas );
	sfml_scrollable_window->Add( table );

	// Create an sf::Sprite for demonstration purposes.
	sf::Texture texture;
	texture.loadFromFile( "data/sfgui.png" );
	sf::Sprite sprite;
	sprite.setTexture( texture );

	// Create an sf::RectangleShape for demonstration purposes.
	sf::RectangleShape rectangle_shape( sf::Vector2f( 218.f * 20, 84.f * 20 ) );
	rectangle_shape.setTexture( &texture );

	const static auto scrollable_canvas_size = 300.f;

	// Create an sf::View to control which part of our rectangle is drawn.
	sf::View view( sf::Vector2f( 100.f, 100.f ), sf::Vector2f( scrollable_canvas_size, scrollable_canvas_size ) );

	// Link the adjustments to our view.
	auto horizontal_adjustment = horizontal_scrollbar->GetAdjustment();
	auto vertical_adjustment = vertical_scrollbar->GetAdjustment();
	horizontal_adjustment->SetLower( scrollable_canvas_size / 2.f );
	horizontal_adjustment->SetUpper( 218.f * 20 - scrollable_canvas_size / 2.f );
	horizontal_adjustment->SetMinorStep( 20.f );
	horizontal_adjustment->SetMajorStep( scrollable_canvas_size );
	horizontal_adjustment->SetPageSize( scrollable_canvas_size );
	vertical_adjustment->SetLower( scrollable_canvas_size / 2.f );
	vertical_adjustment->SetUpper( 84.f * 20 - scrollable_canvas_size / 2.f );
	vertical_adjustment->SetMinorStep( 20.f );
	vertical_adjustment->SetMajorStep( scrollable_canvas_size );
	vertical_adjustment->SetPageSize( scrollable_canvas_size );

	horizontal_adjustment->GetSignal( sfg::Adjustment::OnChange ).Connect( [&view, &horizontal_adjustment]() {
		view.setCenter( horizontal_adjustment->GetValue(), view.getCenter().y );
	} );

	vertical_adjustment->GetSignal( sfg::Adjustment::OnChange ).Connect( [&view, &vertical_adjustment]() {
		view.setCenter( view.getCenter().x, vertical_adjustment->GetValue() );
	} );

	// Because Canvases provide a virtual surface to draw
	// on much like a ScrolledWindow, specifying their
	// minimum requisition is necessary.
	opengl_canvas->SetRequisition( sf::Vector2f( 200.f, 150.f ) );
	sfml_canvas->SetRequisition( sf::Vector2f( texture.getSize() ) );
	sfml_scrollable_canvas->SetRequisition( sf::Vector2f( scrollable_canvas_size, scrollable_canvas_size ) );

	// Create a desktop to contain our Windows.
	sfg::Desktop desktop;
	desktop.Add( opengl_window );
	desktop.Add( sfml_window );
	desktop.Add( sfml_scrollable_window );

	sf::Clock clock;
	sf::Clock rotation_clock;

	// Update an initial time to construct the GUI before drawing begins.
	// This makes sure that there are no frames in which no GUI is visible.
	desktop.Update( 0.f );

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

		// Update the GUI every 5ms
		if( clock.getElapsedTime().asMicroseconds() >= 5000 ) {
			// Update() takes the elapsed time in seconds.
			desktop.Update( static_cast<float>( clock.getElapsedTime().asMicroseconds() ) / 1000000.f );

			clock.restart();
		}

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

		glRotatef( rotation_clock.getElapsedTime().asSeconds() * 30.f, 0.f, 0.f, 1.f );

		glDisable( GL_TEXTURE_2D );

		glMatrixMode( GL_PROJECTION );
		glPushMatrix();
		glLoadIdentity();

		glViewport( 0, 0, static_cast<int>( opengl_canvas->GetAllocation().width ), static_cast<int>( opengl_canvas->GetAllocation().height ) );

		static const auto pi = 3.1415926535897932384626433832795f;
		static const auto fov = 90.f;
		static const auto near_distance = 1.f;
		static const auto far_distance = 20.f;

		auto aspect = opengl_canvas->GetAllocation().width / opengl_canvas->GetAllocation().height;
		auto frustum_height = std::tan( fov / 360 * pi ) * near_distance;
		auto frustum_width = frustum_height * aspect;

		glFrustum( -frustum_width, frustum_width, -frustum_height, frustum_height, near_distance, far_distance );

		glBegin( GL_QUADS );
		glVertex2s( -1, 1 );
		glVertex2s( -1, -1 );
		glVertex2s( 1, -1 );
		glVertex2s( 1, 1 );
		glEnd();

		glPopMatrix();
		glMatrixMode( GL_MODELVIEW );
		glPopMatrix();

		glViewport( 0, 0, static_cast<int>( app_window.getSize().x ), static_cast<int>( app_window.getSize().y ) );

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

		// Then the scrollable Canvas for SFML rendering.
		sfml_scrollable_canvas->Bind();
		sfml_scrollable_canvas->Clear( sf::Color( 0, 0, 0, 0 ) );

		// Draw the SFML Sprite.
		sfml_scrollable_canvas->SetView( view );
		sfml_scrollable_canvas->Draw( rectangle_shape );

		sfml_scrollable_canvas->Display();
		sfml_scrollable_canvas->Unbind();

		// This is important.
		app_window.setActive( true );

		// Draw the GUI
		sfgui.Display( app_window );

		// Update the window
		app_window.display();
	}

	return EXIT_SUCCESS;
}
