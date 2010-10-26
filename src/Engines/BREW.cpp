#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Theme.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Text.hpp>

namespace sfg {
namespace eng {

BREW::BREW() :
	RenderEngine()
{
	// Set defaults.
	SetProperty( "Window.title-height", "15.0" );
	SetProperty( "Window.title-font-size", "10" );
	SetProperty( "Window.title-background-color", "#999999" );
	SetProperty( "Window.background-color", "#888888" );
	SetProperty( "Window.border-width", "2.0" );
	SetProperty( "Window.border-color-light", "#cccccc" );
	SetProperty( "Window.border-color-dark", "#555555" );
	SetProperty( "Window.shadow-distance", "2.0" );
	SetProperty( "Window.shadow-alpha", "100" );

	SetProperty( "Button.border-color-light", "#cccccc" );
	SetProperty( "Button.border-color-dark", "#555555" );
	SetProperty( "Button.background-color", "#999999" );
	SetProperty( "Button:prelight.background-color", "#aaaaaa" );
}

sf::Drawable* BREW::CreateWindowDrawable( Window::Ptr window ) const {
	RenderQueue*  queue( new RenderQueue );
	sf::Color  border_color_light( Theme::ParseColor( GetProperty( "Window.border-color-light", "#ffffff" ) ) );
	sf::Color  border_color_dark( Theme::ParseColor( GetProperty( "Window.border-color-dark", "#000000" ) ) );
	sf::Color  title_background_color( Theme::ParseColor( GetProperty( "Window.title-background-color", "#999999" ) ) );
	float      border_width( GetProperty( "Window.border-width", 1.f ) );
	float      title_size( GetProperty( "Window.title-height", 15.f ) );
	float      shadow_distance( GetProperty( "Window.shadow-distance", 2.f ) );
	sf::Uint8  shadow_alpha( static_cast<sf::Uint8>( GetProperty( "Window.shadow-alpha", 100 ) ) );
	int        title_font_size( GetProperty( "Window.title-font-size", 10 ) );

	if( border_width > 0 ) {
		queue->Add( CreateBorder( window->GetAllocation(), border_width, border_color_light, border_color_dark ) );
	}

	// Shadow.
	if( shadow_distance > 0.f ) {
		sf::Color  shadow_color( 0, 0, 0, shadow_alpha );

		queue->Add( new sf::Shape( sf::Shape::Rectangle( window->GetAllocation().Width + .1f, shadow_distance + .1f, shadow_distance, window->GetAllocation().Height - shadow_distance, shadow_color ) ) ); // Right.
		queue->Add( new sf::Shape( sf::Shape::Rectangle( shadow_distance + .1f, window->GetAllocation().Height + .1f, window->GetAllocation().Width, shadow_distance, shadow_color ) ) ); // Bottom.
	}

	sf::Shape*  background(
		new sf::Shape(
			sf::Shape::Rectangle(
				border_width + .1f,
				border_width + .1f,
				window->GetAllocation().Width - 2 * border_width,
				window->GetAllocation().Height - 2 * border_width,
				Theme::ParseColor( GetProperty( "Window.background-color", "#eeeeee" ) )
			)
		)
	);

	sf::Shape*  title(
		new sf::Shape(
			sf::Shape::Rectangle(
				border_width + .1f,
				border_width + .1f,
				window->GetAllocation().Width - 2 * border_width,
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
		std::floor( border_width + 5.f + .5f ),
		std::floor( border_width + ((title_size / 2.f) - (static_cast<float>( title_font_size ) / 2.f)) + .5f )
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

	queue->Add( new sf::Shape( sf::Shape::Rectangle( rect.Width - border_width + .1f, .1f, border_width, rect.Height, dark_color ) ) ); // Right.
	queue->Add( new sf::Shape( sf::Shape::Rectangle( .1f, rect.Height - border_width + .1f, rect.Width, border_width, dark_color ) ) ); // Bottom.

	for( float delta = 0.f; delta < border_width; delta += 1.f ) {
		queue->Add( new sf::Shape( sf::Shape::Line( .1f, delta + .1f, rect.Width - delta, delta, 1.f, light_color ) ) ); // Top.
		queue->Add( new sf::Shape( sf::Shape::Line( delta + .1f, .1f, delta, rect.Height - delta, 1.f, light_color ) ) ); // Left.
	}
	
	return queue;
}

sf::Drawable* BREW::CreateButtonDrawable( Button::Ptr button ) const {
	sf::Color  border_color_light( Theme::ParseColor( GetProperty( "Button.border-color-light", "#ffffff" ) ) );
	sf::Color  border_color_dark( Theme::ParseColor( GetProperty( "Button.border-color-dark", "#000000" ) ) );
	sf::Color  background_color( Theme::ParseColor( GetProperty( "Button.background-color", "#888888" ) ) );
	sf::Color  prelight_background_color( Theme::ParseColor( GetProperty( "Button:prelight.background-color", "#aaaaaa" ) ) );
	float  border_width( GetProperty( "Button.border-width", 1.f ) );

	RenderQueue*  queue( new RenderQueue );

	queue->Add(
		new sf::Shape(
			sf::Shape::Rectangle(
				0.f,
				0.f,
				button->GetAllocation().Width,
				button->GetAllocation().Height,
				button->GetState() == Widget::Prelight ? prelight_background_color : background_color
			)
		)
	);

	queue->Add( CreateBorder( button->GetAllocation(), border_width, border_color_light, border_color_dark ) );

	// Caption.
	sf::Text*  caption( new sf::Text( button->GetCaption(), sf::Font::GetDefaultFont(), 10.f ) );

	caption->SetPosition(
		sf::Vector2f(
			std::floor( button->GetAllocation().Width / 2.f - caption->GetRect().Width / 2.f + .5f ),
			std::floor( button->GetAllocation().Height / 2.f - caption->GetRect().Height / 2.f + .5f )
		)
	);

	queue->Add( caption );

	return queue;
}

}
}
