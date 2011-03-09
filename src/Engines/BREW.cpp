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
	SetProperty<float>( "Window.Title.Height", 15.f );
	SetProperty<unsigned int>( "Window.Title.FontSize", 10 );
	SetProperty<std::string>( "Window.Title.BackgroundColor", "#999999" );
	SetProperty<std::string>( "Window.BackgroundColor", "#888888" );
	SetProperty<float>( "Window.BorderWidth", 2.f );
	SetProperty<std::string>( "Window.LightBorderColor", "#cccccc" );
	SetProperty<std::string>( "Window.DarkBorderColor", "#555555" );
	SetProperty<float>( "Window.ShadowDistance", 2.f );
	SetProperty<int>( "Window.ShadowAlpha", 100 );

	SetProperty<std::string>( "Button.Normal.LightBorderColor", "#cccccc" );
	SetProperty<std::string>( "Button.Normal.DarkBorderColor", "#555555" );
	SetProperty<std::string>( "Button.Normal.BackgroundColor", "#999999" );
	SetProperty<std::string>( "Button.Normal.TextColor", "#ffffff" );
	SetProperty<std::string>( "Button.Prelight.BackgroundColor", "#aaaaaa" );
	SetProperty<std::string>( "Button.Prelight.TextColor", "#000000" );
	SetProperty<std::string>( "Button.Active.BackgroundColor", "#777777" );
	SetProperty<std::string>( "Button.Active.TextColor", "#000000" );
	SetProperty<std::string>( "Button.Active.LightBorderColor", "#555555" );
	SetProperty<std::string>( "Button.Active.DarkBorderColor", "#cccccc" );

	SetProperty<std::string>( "Label.Font", "" );
	SetProperty<unsigned int>( "Label.FontSize", 14 );
}

sf::Drawable* BREW::CreateWindowDrawable( boost::shared_ptr<Window> window, const sf::RenderTarget& /*target*/ ) const {
	RenderQueue*  queue( new RenderQueue );
	sf::Color  background_color( Theme::ParseColor( GetWidgetProperty<std::string>( window, "Window.BackgroundColor", "#eeeeee" ) ) );
	sf::Color  border_color_light( Theme::ParseColor( GetWidgetProperty<std::string>( window, "Window.LightBorderColor", "#ffffff" ) ) );
	sf::Color  border_color_dark( Theme::ParseColor( GetWidgetProperty<std::string>( window, "Window.DarkBorderColor", "#000000" ) ) );
	sf::Color  title_background_color( Theme::ParseColor( GetWidgetProperty<std::string>( window, "Window.Title.BackgroundColor", "#999999" ) ) );
	float      border_width( GetWidgetProperty<float>( window, "Window.BorderWidth", 1.f ) );
	float      title_size( GetWidgetProperty<float>( window, "Window.Title.Height", 15.f ) );
	float      shadow_distance( GetWidgetProperty<float>( window, "Window.ShadowDistance", 2.f ) );
	sf::Uint8  shadow_alpha( GetWidgetProperty<sf::Uint8>( window, "Window.ShadowAlpha", 100 ) );
	int        title_font_size( GetWidgetProperty<unsigned int>( window, "Window.Title.FontSize", 10 ) );

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
	sf::Color  border_color_light( Theme::ParseColor( GetWidgetProperty<std::string>( button, "Button.Normal.LightBorderColor", "#ffffff" ) ) );
	sf::Color  border_color_dark( Theme::ParseColor( GetWidgetProperty<std::string>( button, "Button.Normal.DarkBorderColor", "#000000" ) ) );
	sf::Color  background_color( Theme::ParseColor( GetWidgetProperty<std::string>( button, "Button.Normal.BackgroundColor", "#888888" ) ) );
	sf::Color  text_color( Theme::ParseColor( GetWidgetProperty<std::string>( button, "Button.Normal.TextColor", "#ffffff" ) ) );
	float  border_width( GetWidgetProperty( button, "Button.Normal.BorderWidth", 1.f ) );

	if( button->GetState() == Widget::Prelight ) {
		background_color = Theme::ParseColor( GetWidgetProperty<std::string>( button, "Button.Prelight.BackgroundColor", "#888888" ) );
		text_color = Theme::ParseColor( GetWidgetProperty<std::string>( button, "Button.Prelight.TextColor", "#ffffff" ) );
	}
	else if( button->GetState() == Widget::Active ) {
		background_color = Theme::ParseColor( GetWidgetProperty<std::string>( button, "Button.Active.BackgroundColor", "#ffffbb" ) );
		text_color = Theme::ParseColor( GetWidgetProperty<std::string>( button, "Button.Active.TextColor", "#000000" ) );
		border_color_light = Theme::ParseColor( GetWidgetProperty<std::string>( button, "Button.Active.LightBorderColor", "#000000" ) );
		border_color_dark = Theme::ParseColor( GetWidgetProperty<std::string>( button, "Button.Active.DarkBorderColor", "#ffffff" ) );
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
	const sf::Font&  font( LoadFontFromFile( GetWidgetProperty<std::string>( label, "Label.Font", "" ) ) );
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
