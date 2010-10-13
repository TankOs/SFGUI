#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Theme.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Text.hpp>

namespace sfg {
namespace eng {

sf::Drawable* BREW::CreateWindowDrawable( Window::Ptr window ) const {
	RenderQueue*  queue( new RenderQueue );
	sf::Color  border_color_light( Theme::ParseColor( GetProperty( "Window.border-color-light", "#ffffff" ) ) );
	sf::Color  border_color_dark( Theme::ParseColor( GetProperty( "Window.border-color-dark", "#000000" ) ) );
	sf::Color  title_background_color( Theme::ParseColor( GetProperty( "Window.title-background-color", "#999999" ) ) );
	float      border_width( GetProperty( "Window.border-width", 1.f ) );
	float      title_size( GetProperty( "Window.title-size", 10.f ) );
	float      shadow_distance( GetProperty( "Window.shadow-distance", 2.f ) );
	sf::Uint8  shadow_alpha( static_cast<sf::Uint8>( GetProperty( "Window.shadow-alpha", 100 ) ) );
	int        title_font_size( GetProperty( "Window.title-font-size", 10 ) );

	if( border_width > 0 ) {
		queue->Add( CreateBorder( window->GetAllocation(), border_width, border_color_light, border_color_dark ) );
	}

	// Shadow.
	if( shadow_distance > 0.f ) {
		sf::Color  shadow_color( 0, 0, 0, shadow_alpha );

		queue->Add( new sf::Shape( sf::Shape::Rectangle( window->GetAllocation().Width, shadow_distance, shadow_distance, window->GetAllocation().Height - shadow_distance, shadow_color ) ) ); // Right.
		queue->Add( new sf::Shape( sf::Shape::Rectangle( shadow_distance, window->GetAllocation().Height, window->GetAllocation().Width, shadow_distance, shadow_color ) ) ); // Bottom.
	}

	sf::Shape*  background(
		new sf::Shape(
			sf::Shape::Rectangle(
				border_width - 1, // FIXME: SFML bug!
				border_width,
				window->GetAllocation().Width - 2 * border_width + 1, // FIXME: SFML bug!
				window->GetAllocation().Height - 2 * border_width,
				Theme::ParseColor( GetProperty( "Window.background-color", "#eeeeee" ) )
			)
		)
	);

	sf::Shape*  title(
		new sf::Shape(
			sf::Shape::Rectangle(
				border_width - 1, // FIXME: SFML bug!
				border_width,
				window->GetAllocation().Width - 2 * border_width + 1, // FIXME: SFML bug!
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

RenderQueue* BREW::CreateBorder( const sf::FloatRect& rect, float border_width, const sf::Color& light_color, const sf::Color& dark_color ) {
	RenderQueue* queue( new RenderQueue );

	queue->Add( new sf::Shape( sf::Shape::Rectangle( rect.Width - border_width, 0.f, border_width, rect.Height, dark_color ) ) ); // Right.
	queue->Add( new sf::Shape( sf::Shape::Rectangle( 0.f, rect.Height - border_width, rect.Width, border_width, dark_color ) ) ); // Bottom.

	for( float delta = 0.f; delta < border_width; delta += 1.f ) {
		queue->Add( new sf::Shape( sf::Shape::Line( 0.f, delta, rect.Width - delta, delta, 1.f, light_color ) ) ); // Top.
		queue->Add( new sf::Shape( sf::Shape::Line( delta, 0.f, delta, rect.Height - delta, 1.f, light_color ) ) ); // Left.
	}
	
	return queue;
}

sf::Drawable* BREW::CreateButtonDrawable( Button::Ptr button ) const {
	RenderQueue*  queue( new RenderQueue );

	queue->Add( CreateBorder( button->GetAllocation(), 2.f, sf::Color( 255, 255, 255 ), sf::Color( 100, 100, 100 ) ) );

	return queue;
}

}
}
