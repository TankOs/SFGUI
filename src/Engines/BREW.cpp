#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Theme.hpp>
#include <SFGUI/Window.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Label.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Text.hpp>

namespace sfg {
namespace eng {

BREW::BREW() :
	RenderEngine()
{
	// Set defaults.
	SetProperty( "Window.Title.Height", 15.f );
	SetProperty( "Window.Title.FontSize", 10 );
	SetProperty( "Window.Title.BackgroundColor", sf::Color( 0x99, 0x99, 0x99 ) );
	SetProperty( "Window.BackgroundColor", sf::Color( 0x88, 0x88, 0x88 ) );
	SetProperty( "Window.BorderWidth", 2.f );
	SetProperty( "Window.LightBorderColor", sf::Color( 0xCC, 0xCC, 0xCC ) );
	SetProperty( "Window.DarkBorderColor", sf::Color( 0x55, 0x55, 0x55 ) );
	SetProperty( "Window.ShadowDistance", 2.f );
	SetProperty<sf::Uint8>( "Window.ShadowAlpha", 100 );

	SetProperty( "Button.Normal.LightBorderColor", sf::Color( 0xCC, 0xCC, 0xCC ) );
	SetProperty( "Button.Normal.DarkBorderColor", sf::Color( 0x55, 0x55, 0x55 ) );
	SetProperty( "Button.Normal.BackgroundColor", sf::Color( 0x99, 0x99, 0x99 ) );
	SetProperty( "Button.Normal.TextColor", sf::Color( 0xFF, 0xFF, 0xFF ) );
	SetProperty( "Button.Prelight.BackgroundColor", sf::Color( 0xAA, 0xAA, 0xAA ) );
	SetProperty( "Button.Prelight.TextColor", sf::Color( 0x00, 0x00, 0x00 ) );
	SetProperty( "Button.Active.BackgroundColor", sf::Color( 0x77, 0x77, 0x77 ) );
	SetProperty( "Button.Active.TextColor", sf::Color( 0x00, 0x00, 0x00 ) );
	SetProperty( "Button.Active.LightBorderColor", sf::Color( 0x55, 0x55, 0x55 ) );
	SetProperty( "Button.Active.DarkBorderColor", sf::Color( 0xCC, 0xCC, 0xCC ) );

	SetProperty( "Label.Font", std::string( "" ) );
	SetProperty<unsigned int>( "Label.FontSize", 12 );
}

sf::Drawable* BREW::CreateWindowDrawable( boost::shared_ptr<Window> window, const sf::RenderTarget& /*target*/ ) const {
	RenderQueue* queue( new RenderQueue );
	sf::Color background_color( GetWidgetProperty( window, "Window.BackgroundColor", sf::Color( 0xEE, 0xEE, 0xEE ) ) );
	sf::Color border_color_light( GetWidgetProperty( window, "Window.LightBorderColor", sf::Color( 0xFF, 0xFF, 0xFF ) ) );
	sf::Color border_color_dark( GetWidgetProperty( window, "Window.DarkBorderColor", sf::Color( 0x00, 0x00, 0x00 ) ) );
	sf::Color title_background_color( GetWidgetProperty( window, "Window.Title.BackgroundColor", sf::Color( 0x99, 0x99, 0x99 ) ) );
	float border_width( GetWidgetProperty( window, "Window.BorderWidth", 1.f ) );
	float title_size( GetWidgetProperty( window, "Window.Title.Height", 15.f ) );
	float shadow_distance( GetWidgetProperty( window, "Window.ShadowDistance", 2.f ) );
	sf::Uint8 shadow_alpha( GetWidgetProperty( window, "Window.ShadowAlpha", sf::Uint8( 100 ) ) );
	int title_font_size( GetWidgetProperty( window, "Window.Title.FontSize", 10 ) );

	if( window->HasStyle( Window::Background ) ) {
		// Shadow.
		if( shadow_distance > 0.f ) {
			sf::Color  shadow_color( 0, 0, 0, shadow_alpha );

			queue->Add( new sf::Shape( sf::Shape::Rectangle( window->GetAllocation().Width + .1f, shadow_distance + .1f, shadow_distance, window->GetAllocation().Height - shadow_distance, shadow_color ) ) ); // Right.
			queue->Add( new sf::Shape( sf::Shape::Rectangle( shadow_distance + .1f, window->GetAllocation().Height + .1f, window->GetAllocation().Width, shadow_distance, shadow_color ) ) ); // Bottom.
		}

		if( border_width > 0 ) {
			queue->Add( CreateBorder( window->GetAllocation(), border_width, border_color_light, border_color_dark ) );
		}

		sf::Shape*  background(
			new sf::Shape(
				sf::Shape::Rectangle(
					border_width + .1f,
					border_width + .1f,
					window->GetAllocation().Width - 2 * border_width,
					window->GetAllocation().Height - 2 * border_width,
					background_color
				)
			)
		);

		queue->Add( background );
	}

	if( !window->HasStyle( Window::Titlebar ) ) {
		title_size = 0;
	}

	if( title_size > 0 ) {
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

		queue->Add( title );
		queue->Add( title_text );
	}


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

sf::Drawable* BREW::CreateButtonDrawable( boost::shared_ptr<Button> button, const sf::RenderTarget& /*target*/ ) const {
	sf::Color  border_color_light( GetWidgetProperty( button, "Button.Normal.LightBorderColor", sf::Color( 0xFF, 0xFF, 0xFF ) ) );
	sf::Color  border_color_dark( GetWidgetProperty( button, "Button.Normal.DarkBorderColor", sf::Color( 0x00, 0x00, 0x00 ) ) );
	sf::Color  background_color( GetWidgetProperty( button, "Button.Normal.BackgroundColor", sf::Color( 0x88, 0x88, 0x88 ) ) );
	sf::Color  text_color( GetWidgetProperty( button, "Button.Normal.TextColor", sf::Color( 0xFF, 0xFF, 0xFF ) ) );
	float  border_width( GetWidgetProperty( button, "Button.Normal.BorderWidth", 1.f ) );

	if( button->GetState() == Widget::Prelight ) {
		background_color = GetWidgetProperty( button, "Button.Prelight.BackgroundColor", sf::Color( 0x88, 0x88, 0x88 ) );
		text_color = GetWidgetProperty( button, "Button.Prelight.TextColor", sf::Color( 0xFF, 0xFF, 0xFF ) );
	}
	else if( button->GetState() == Widget::Active ) {
		background_color = GetWidgetProperty( button, "Button.Active.BackgroundColor", sf::Color( 0xFF, 0xFF, 0xBB ) );
		text_color = GetWidgetProperty( button, "Button.Active.TextColor", sf::Color( 0x00, 0x00, 0x00 ) );
		border_color_light = GetWidgetProperty( button, "Button.Active.LightBorderColor", sf::Color( 0x00, 0x00, 0x00 ) );
		border_color_dark = GetWidgetProperty( button, "Button.Active.DarkBorderColor", sf::Color( 0xFF, 0xFF, 0xFF ) );
	}

	RenderQueue*  queue( new RenderQueue );

	queue->Add(
		new sf::Shape(
			sf::Shape::Rectangle(
				0.f,
				0.f,
				button->GetAllocation().Width,
				button->GetAllocation().Height,
				background_color
			)
		)
	);

	queue->Add( CreateBorder( button->GetAllocation(), border_width, border_color_light, border_color_dark ) );

	return queue;
}

sf::Drawable* BREW::CreateLabelDrawable( boost::shared_ptr<Label> label, const sf::RenderTarget& /*target*/ ) const {
	const sf::Font&  font( LoadFontFromFile( GetWidgetProperty( label, "Label.Font", std::string() ) ) );
	const unsigned int&  font_size( GetWidgetProperty<unsigned int>( label, "Label.FontSize", 10 ) );

	sf::Text*  vis_label( new sf::Text( label->GetText(), font, font_size ) );

	// Calculate alignment.
	sf::Vector2f  avail_space( label->GetAllocation().Width - label->GetRequisition().x, label->GetAllocation().Height - label->GetRequisition().y );
	sf::Vector2f  position( avail_space.x * label->GetAlignment().x, avail_space.y * label->GetAlignment().y );

	vis_label->SetPosition( std::floor( position.x + .5f ), std::floor( position.y + .5f ) );

	RenderQueue*  queue( new RenderQueue );
	queue->Add( vis_label );

	return queue;
}

}
}
