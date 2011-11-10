#include <SFGUI/SFGUI.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

const float PI = 3.14159265f;

int main() {
	sf::RenderWindow render_window( sf::VideoMode( 800, 600 ), "SFGUI with OpenGL example" );

	sfg::Scale::Ptr red_scale( sfg::Scale::Create( 0.f, 1.f, .01f, sfg::Scale::Horizontal ) );
	sfg::Scale::Ptr green_scale( sfg::Scale::Create( 0.f, 1.f, .01f, sfg::Scale::Horizontal ) );
	sfg::Scale::Ptr blue_scale( sfg::Scale::Create( 0.f, 1.f, .01f, sfg::Scale::Horizontal ) );
	sfg::Scale::Ptr angle_scale( sfg::Scale::Create( 0.f, 360.f, 1.f, sfg::Scale::Horizontal ) );
	sfg::CheckButton::Ptr auto_check( sfg::CheckButton::Create( "Auto" ) );

	sfg::Table::Ptr table( sfg::Table::Create() );
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

	sfg::Window::Ptr window( sfg::Window::Create() );
	window->SetTitle( "SFGUI with OpenGL" );
	window->SetBorderWidth( 10.f );
	window->Add( table );

	sfg::Desktop desktop( render_window );
	desktop.Add( window );

	red_scale->SetValue( .2f );
	green_scale->SetValue( .5f );
	blue_scale->SetValue( .8f );

	//window->AllocateSize( sf::FloatRect( 0.f, 0.f, 300.f, 400.f ) );

	render_window.SetActive();

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 90.f, 800.f / 600.f, .1f, 100.f );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	sf::Event event;

	while( render_window.IsOpened() ) {
		while( render_window.PollEvent( event ) ) {
			if( event.Type == sf::Event::Closed ) {
				render_window.Close();
			}
			else {
				desktop.HandleEvent( event );
			}
		}

		if( auto_check->IsActive() ) {
			float angle( angle_scale->GetValue() );
			angle += static_cast<float>( render_window.GetFrameTime() ) * .5f;

			while( angle >= 360.f ) {
				angle -= 360.f;
			}

			angle_scale->SetValue( angle );
		}

		render_window.Clear();

		render_window.RestoreGLStates();
		render_window.SetActive();

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



		// SFML rendering.
		render_window.SaveGLStates();

		desktop.Expose( render_window );

		render_window.Display();
	}

	return 0;
}
