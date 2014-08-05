#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

int main() {
	// An sf::Window for raw OpenGL rendering.
	sf::Window app_window( sf::VideoMode( 800, 600 ), "SFGUI with OpenGL example", sf::Style::Titlebar | sf::Style::Close );

	// Create an SFGUI. This is required before doing anything with SFGUI.
	sfg::SFGUI sfgui;

	// Set the SFML Window's context back to the active one. SFGUI creates
	// a temporary context on creation that is set active.
	app_window.setActive();

	// Initial OpenGL setup.
	// We have to set up our own OpenGL viewport because we are using an sf::Window.
	glViewport( 0, 0, static_cast<int>( app_window.getSize().x ), static_cast<int>( app_window.getSize().y ) );

	auto red_scale = sfg::Scale::Create( 0.f, 1.f, .01f, sfg::Scale::Orientation::HORIZONTAL );
	auto green_scale = sfg::Scale::Create( 0.f, 1.f, .01f, sfg::Scale::Orientation::HORIZONTAL );
	auto blue_scale = sfg::Scale::Create( 0.f, 1.f, .01f, sfg::Scale::Orientation::HORIZONTAL );
	auto angle_scale = sfg::Scale::Create( 0.f, 360.f, 1.f, sfg::Scale::Orientation::HORIZONTAL );
	auto auto_check = sfg::CheckButton::Create( "Auto" );

	auto table = sfg::Table::Create();
	table->SetRowSpacings( 5.f );
	table->SetColumnSpacings( 5.f );

	table->Attach( sfg::Label::Create( "Change the color of the rect using the scales below." ), sf::Rect<sf::Uint32>( 0, 0, 3, 1 ), sfg::Table::FILL, sfg::Table::FILL );
	table->Attach( sfg::Label::Create( "Red:" ), sf::Rect<sf::Uint32>( 0, 1, 1, 1 ), sfg::Table::FILL, sfg::Table::FILL );
	table->Attach( red_scale, sf::Rect<sf::Uint32>( 1, 1, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL | sfg::Table::EXPAND );
	table->Attach( sfg::Label::Create( "Green:" ), sf::Rect<sf::Uint32>( 0, 2, 1, 1 ), sfg::Table::FILL, sfg::Table::FILL );
	table->Attach( green_scale, sf::Rect<sf::Uint32>( 1, 2, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL | sfg::Table::EXPAND );
	table->Attach( sfg::Label::Create( "Blue:" ), sf::Rect<sf::Uint32>( 0, 3, 1, 1 ), sfg::Table::FILL, sfg::Table::FILL );
	table->Attach( blue_scale, sf::Rect<sf::Uint32>( 1, 3, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL | sfg::Table::EXPAND );
	table->Attach( sfg::Label::Create( "Angle:" ), sf::Rect<sf::Uint32>( 0, 4, 1, 1 ), sfg::Table::FILL, sfg::Table::FILL );
	table->Attach( angle_scale, sf::Rect<sf::Uint32>( 1, 4, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL | sfg::Table::EXPAND );
	table->Attach( auto_check, sf::Rect<sf::Uint32>( 2, 4, 1, 1 ), sfg::Table::FILL, sfg::Table::FILL );

	auto window = sfg::Window::Create();
	window->SetTitle( "SFGUI with OpenGL" );
	window->Add( table );

	sfg::Desktop desktop;
	desktop.Add( window );

	red_scale->SetValue( .2f );
	green_scale->SetValue( .5f );
	blue_scale->SetValue( .8f );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	static const auto pi = 3.1415926535897932384626433832795f;
	static const auto fov = 90.f;
	static const auto near_distance = .1f;
	static const auto far_distance = 100.f;
	static const auto aspect = 800.f / 600.f;

	auto frustum_height = std::tan( fov / 360 * pi ) * near_distance;
	auto frustum_width = frustum_height * aspect;

	glFrustum( -frustum_width, frustum_width, -frustum_height, frustum_height, near_distance, far_distance );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	sf::Event event;

	sf::Clock clock;

	while( app_window.isOpen() ) {
		auto delta = clock.restart().asSeconds();

		while( app_window.pollEvent( event ) ) {
			if( event.type == sf::Event::Closed ) {
				app_window.close();
			}
			else {
				desktop.HandleEvent( event );
			}
		}

		if( auto_check->IsActive() ) {
			float angle( angle_scale->GetValue() );
			angle += delta * 90.f;

			while( angle >= 360.f ) {
				angle -= 360.f;
			}

			angle_scale->SetValue( angle );
		}

		glClear( GL_COLOR_BUFFER_BIT );

		glMatrixMode( GL_MODELVIEW );

		glLoadIdentity();
		glRotatef( angle_scale->GetValue(), 0.f, 0.f, 1.f );
		glTranslatef( -.5f, -.5f, -5.f );

		glBegin( GL_QUADS );
		glColor3f( red_scale->GetValue(), green_scale->GetValue(), blue_scale->GetValue() );
		glVertex3f( 0.f, 1.f, 0.f );
		glVertex3f( 0.f, 0.f, 0.f );
		glVertex3f( 1.f, 0.f, 0.f );
		glVertex3f( 1.f, 1.f, 0.f );
		glEnd();

		desktop.Update( delta );

		// SFGUI rendering.
		sfgui.Display( app_window );

		app_window.display();
	}

	return 0;
}
