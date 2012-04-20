#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Window.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Context.hpp>

#include <SFML/Graphics/Text.hpp>

namespace sfg {
namespace eng {

RenderQueue* BREW::CreateWindowDrawable( SharedPtr<const Window> window ) const {
	RenderQueue* queue( new RenderQueue );
	sf::Color background_color( GetProperty<sf::Color>( "BackgroundColor", window ) );
	sf::Color border_color( GetProperty<sf::Color>( "BorderColor", window ) );
	sf::Color title_background_color( GetProperty<sf::Color>( "TitleBackgroundColor", window ) );
	sf::Color title_text_color( GetProperty<sf::Color>( "Color", window ) );
	int border_color_shift( GetProperty<int>( "BorderColorShift", window ) );
	float border_width( GetProperty<float>( "BorderWidth", window ) );
	float title_padding( GetProperty<float>( "TitlePadding", window ) );
	float shadow_distance( GetProperty<float>( "ShadowDistance", window ) );
	float handle_size( GetProperty<float>( "HandleSize", window ) );
	sf::Uint8 shadow_alpha( GetProperty<sf::Uint8>( "ShadowAlpha", window ) );
	unsigned int title_font_size( GetProperty<unsigned int>( "FontSize", window ) );
	const sf::Font& title_font( *GetResourceManager().GetFont( GetProperty<std::string>( "FontName", window ) ) );
	float title_size( GetFontLineHeight( title_font, title_font_size ) + 2 * title_padding );

	if( window->HasStyle( Window::SHADOW ) ) {
		// Shadow.
		sf::Color shadow_color( 0, 0, 0, shadow_alpha );

		sf::FloatRect shadow_rect(
			shadow_distance,
			shadow_distance,
			window->GetAllocation().width,
			window->GetAllocation().height
		);

		queue->Add(
			Renderer::Get().CreateRect(
				shadow_rect,
				shadow_color
			)
		);
	}

	if( window->HasStyle( Window::BACKGROUND ) ) {
		// Pane.
		queue->Add(
			Renderer::Get().CreatePane(
				sf::Vector2f( 0.f, 0.f ),
				sf::Vector2f( window->GetAllocation().width, window->GetAllocation().height ),
				border_width,
				background_color,
				border_color,
				border_color_shift
			)
		);
	}

	if( window->HasStyle( Window::RESIZE ) ) {
		queue->Add(
			Renderer::Get().CreateTriangle(
				sf::Vector2f( window->GetAllocation().width, window->GetAllocation().height - handle_size ),
				sf::Vector2f( window->GetAllocation().width - handle_size, window->GetAllocation().height ),
				sf::Vector2f( window->GetAllocation().width, window->GetAllocation().height ),
				title_background_color
			)
		);
	}


	if( !window->HasStyle( Window::TITLEBAR ) ) {
		title_size = 0;
	}

	if( title_size > 0 ) {
		queue->Add(
			Renderer::Get().CreateRect(
				sf::FloatRect(
					border_width + .1f,
					border_width + .1f,
					window->GetAllocation().width - 2 * border_width,
					title_size
				),
				title_background_color
			)
		);

		// Find out visible text, count in "...".
		float avail_width( window->GetAllocation().width - 2.f * border_width - 2.f * title_padding );

		sf::Text title_text( window->GetTitle(), title_font, title_font_size );

		if( title_text.getLocalBounds().width > avail_width ) {
			sf::Text dots( "...", title_font, title_font_size );
			const sf::String& title_string( window->GetTitle() );
			sf::String visible_title;

			avail_width = window->GetAllocation().width - 2.f * border_width - 2.f * title_padding - dots.getLocalBounds().width;

			for( std::size_t ch_index = 0; ch_index < title_string.getSize(); ++ch_index ) {
				avail_width -= static_cast<float>( title_font.getGlyph( title_string[ch_index], title_font_size, false ).advance );

				if( avail_width < 0.f ) {
					visible_title += "...";
					break;
				}

				visible_title += title_string[ch_index];
			}

			title_text.setString( visible_title );
		}

		// Calculate title text position.
		sf::Vector2f title_position(
			border_width + title_padding,
			border_width + title_size / 2.f - static_cast<float>( title_font_size ) / 2.f
		);

		title_text.setPosition( title_position );
		title_text.setColor( title_text_color );

		queue->Add( Renderer::Get().CreateText( title_text ) );
	}

	return queue;
}

}
}
