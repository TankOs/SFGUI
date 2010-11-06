#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Theme.hpp>
#include <SFGUI/Window.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/Scissor.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Text.hpp>

namespace sfg {
namespace eng {

BREW::BREW() :
	RenderEngine()
{
	// Set defaults.
	SetProperty<float>( "Window.title-height", 15.f );
	SetProperty<unsigned int>( "Window.title-font-size", 10 );
	SetProperty<std::string>( "Window.title-background-color", "#999999" );
	SetProperty<std::string>( "Window.background-color", "#888888" );
	SetProperty<float>( "Window.border-width", 2.f );
	SetProperty<std::string>( "Window.border-color-light", "#cccccc" );
	SetProperty<std::string>( "Window.border-color-dark", "#555555" );
	SetProperty<float>( "Window.shadow-distance", 2.f );
	SetProperty<int>( "Window.shadow-alpha", 100 );

	SetProperty<std::string>( "Button.border-color-light", "#cccccc" );
	SetProperty<std::string>( "Button.border-color-dark", "#555555" );
	SetProperty<std::string>( "Button.background-color", "#999999" );
	SetProperty<std::string>( "Button:prelight.background-color", "#aaaaaa" );
	SetProperty<std::string>( "Button:active.background-color", "#ffffbb" );
	SetProperty<std::string>( "Button.text-color", "#ffffff" );
	SetProperty<std::string>( "Button:prelight.text-color", "#ffffff" );
	SetProperty<std::string>( "Button:active.text-color", "#000000" );

	SetProperty<std::string>( "Label.font-family", "arial.ttf" );
	SetProperty<unsigned int>( "Label.font-size", 10 );
}

sf::Drawable* BREW::CreateWindowDrawable( boost::shared_ptr<Window> window, const sf::RenderTarget& /*target*/ ) const {
	RenderQueue*  queue( new RenderQueue );
	sf::Color  background_color( Theme::ParseColor( GetWidgetProperty<std::string>( window, "Window.background-color", "#eeeeee" ) ) );
	sf::Color  border_color_light( Theme::ParseColor( GetWidgetProperty<std::string>( window, "Window.border-color-light", "#ffffff" ) ) );
	sf::Color  border_color_dark( Theme::ParseColor( GetWidgetProperty<std::string>( window, "Window.border-color-dark", "#000000" ) ) );
	sf::Color  title_background_color( Theme::ParseColor( GetWidgetProperty<std::string>( window, "Window.title-background-color", "#999999" ) ) );
	float      border_width( GetWidgetProperty<float>( window, "Window.border-width", 1.f ) );
	float      title_size( GetWidgetProperty<float>( window, "Window.title-height", 15.f ) );
	float      shadow_distance( GetWidgetProperty<float>( window, "Window.shadow-distance", 2.f ) );
	sf::Uint8  shadow_alpha( GetWidgetProperty<sf::Uint8>( window, "Window.shadow-alpha", 100 ) );
	int        title_font_size( GetWidgetProperty<unsigned int>( window, "Window.title-font-size", 10 ) );

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
	sf::Color  border_color_light( Theme::ParseColor( GetWidgetProperty<std::string>( button, "Button.border-color-light", "#ffffff" ) ) );
	sf::Color  border_color_dark( Theme::ParseColor( GetWidgetProperty<std::string>( button, "Button.border-color-dark", "#000000" ) ) );
	sf::Color  background_color( Theme::ParseColor( GetWidgetProperty<std::string>( button, "Button.background-color", "#888888" ) ) );
	sf::Color  text_color( Theme::ParseColor( GetWidgetProperty<std::string>( button, "Button.text-color", "#ffffff" ) ) );
	float  border_width( GetWidgetProperty( button, "Button.border-width", 1.f ) );

	if( button->GetState() == Widget::Prelight ) {
		background_color = Theme::ParseColor( GetWidgetProperty<std::string>( button, "Button:prelight.background-color", "#888888" ) );
		text_color = Theme::ParseColor( GetWidgetProperty<std::string>( button, "Button:prelight.text-color", "#ffffff" ) );
	}
	else if( button->GetState() == Widget::Active ) {
		background_color = Theme::ParseColor( GetWidgetProperty<std::string>( button, "Button:active.background-color", "#ffffbb" ) );
		text_color = Theme::ParseColor( GetWidgetProperty<std::string>( button, "Button:active.text-color", "#000000" ) );
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

	// Caption.
	/*sf::Text*  caption( new sf::Text( button->GetLabel(), sf::Font::GetDefaultFont(), 10.f ) );

	caption->SetColor( text_color );
	caption->SetPosition(
		sf::Vector2f(
			std::floor( button->GetAllocation().Width / 2.f - caption->GetRect().Width / 2.f + .5f ),
			std::floor( button->GetAllocation().Height / 2.f - caption->GetRect().Height / 2.f + .5f )
		)
	);

	queue->Add( caption );*/

	return queue;
}

sf::Drawable* BREW::CreateLabelDrawable( boost::shared_ptr<Label> label, const sf::RenderTarget& target ) const {
	const sf::Font&  font( LoadFontFromFile( GetWidgetProperty<std::string>( label, "Label.font-family", "arial.ttf" ) ) );
	const unsigned int&  font_size( GetWidgetProperty<unsigned int>( label, "Label.font-size", 10 ) );

	RenderQueue*  queue( new RenderQueue );
	sf::IntRect  scree_rect(
		0,
		0,
		target.GetWidth(),
		target.GetHeight()
	);

	queue->Add(
		new Scissor(
			true,
			sf::FloatRect(
				label->GetAllocation().Left,
				label->GetAllocation().Top,
				label->GetAllocation().Width,
				label->GetAllocation().Height
			)
		)
	);

	std::cout << label->GetAllocation().Left << ", " << label->GetAllocation().Top << ", " << label->GetAllocation().Width << ", " << label->GetAllocation().Height << std::endl;

	sf::Text*  vis_label( new sf::Text( label->GetText(), font, font_size ) );

	queue->Add( vis_label );
	queue->Add( new Scissor( false ) );

	return queue;
}

const sf::Font& BREW::LoadFontFromFile( const std::string& filename ) const {
	FontsMap::const_iterator  iter( m_fonts.find( filename ) );

	if( iter != m_fonts.end() ) {
		return iter->second;
	}

	sf::Font  font;
	if( !font.LoadFromFile( filename ) ) {
		return sf::Font::GetDefaultFont();
	}

	m_fonts[filename] = font;
	return m_fonts[filename];
}

}
}
