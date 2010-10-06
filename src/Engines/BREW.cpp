#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/RenderQueue.hpp>
#include <SFGUI/Theme.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Text.hpp>

namespace sfg {
namespace eng {

sf::Drawable* BREW::CreateWindowDrawable( Window::Ptr window ) const {
	RenderQueue*  queue( new RenderQueue );
	sf::Color  border_color_light( Theme::ParseColor( GetProperty( "Window.border-color-light" ) ) );
	sf::Color  border_color_dark( Theme::ParseColor( GetProperty( "Window.border-color-dark" ) ) );
	sf::Color  title_background_color( Theme::ParseColor( GetProperty( "Window.title-background-color" ) ) );
	float      border_width( static_cast<float>( Theme::ParseInt( GetProperty( "Window.border-width", "1" ), 1 ) ) );
	float      title_size( static_cast<float>( Theme::ParseInt( GetProperty( "Window.title-size", "10" ), 10 ) ) );
	float      shadow_distance( static_cast<float>( Theme::ParseInt( GetProperty( "Window.shadow-distance", "2" ), 2 ) ) );
	sf::Uint8  shadow_alpha( static_cast<sf::Uint8>( Theme::ParseInt( GetProperty( "Window.shadow-alpha", "100" ), 100 ) ) );
	int        title_font_size( Theme::ParseInt( GetProperty( "Window.title-font-size", "10" ), 10 ) );

	if( border_width > 0 ) {
		queue->Add( new sf::Shape( sf::Shape::Rectangle( window->GetRect().Width - border_width, 0.f, border_width, window->GetRect().Height, border_color_dark ) ) ); // Right.
		queue->Add( new sf::Shape( sf::Shape::Rectangle( 0.f, window->GetRect().Height - border_width, window->GetRect().Width, border_width, border_color_dark ) ) ); // Bottom.

		for( float delta = 0.f; delta < border_width; delta += 1.f ) {
			queue->Add( new sf::Shape( sf::Shape::Line( 0.f, delta, window->GetRect().Width - delta, delta, 1.f, border_color_light ) ) ); // Top.
			queue->Add( new sf::Shape( sf::Shape::Line( delta, 0.f, delta, window->GetRect().Height - delta, 1.f, border_color_light ) ) ); // Left.
		}
	}

	// Shadow.
	if( shadow_distance > 0.f ) {
		sf::Color  shadow_color( 0, 0, 0, shadow_alpha );

		queue->Add( new sf::Shape( sf::Shape::Rectangle( window->GetRect().Width, shadow_distance, shadow_distance, window->GetRect().Height - shadow_distance, shadow_color ) ) ); // Right.
		queue->Add( new sf::Shape( sf::Shape::Rectangle( shadow_distance, window->GetRect().Height, window->GetRect().Width, shadow_distance, shadow_color ) ) ); // Bottom.
	}

	sf::Shape*  background(
		new sf::Shape(
			sf::Shape::Rectangle(
				border_width - 1, // FIXME: SFML bug!
				border_width,
				window->GetRect().Width - 2 * border_width + 1, // FIXME: SFML bug!
				window->GetRect().Height - 2 * border_width,
				Theme::ParseColor( GetProperty( "Window.background-color", "#eeeeee" ) )
			)
		)
	);

	sf::Shape*  title(
		new sf::Shape(
			sf::Shape::Rectangle(
				border_width - 1, // FIXME: SFML bug!
				border_width,
				window->GetRect().Width - 2 * border_width + 1, // FIXME: SFML bug!
				title_size,
				title_background_color
			)
		)
	);

	sf::Text*  title_text(
		new sf::Text(
			window->GetTitle(),
			sf::Font::GetDefaultFont(),
			title_font_size
		)
	);

	// Calculate title text position.
	sf::Vector2f  title_position(
		border_width + 5.f,
		border_width + ((title_size / 2.f) - (static_cast<float>( title_font_size ) / 2.f))
	);

	title_text->SetPosition( title_position );
	title_text->SetColor( sf::Color( 0, 0, 0 ) );

	queue->Add( background );
	queue->Add( title );
	queue->Add( title_text );

	return queue;
}

}
}
