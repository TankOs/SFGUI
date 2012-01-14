#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Window.hpp>
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
	float title_size( GetLineHeight( title_font, title_font_size ) + 2 * title_padding );

	if( window->HasStyle( Window::Background ) ) {
		// Shadow.
		if( window->HasStyle( Window::Shadow ) ) {
			sf::Color shadow_color( 0, 0, 0, shadow_alpha );

			sf::FloatRect shadow_rect(
				shadow_distance,
				shadow_distance,
				window->GetAllocation().Width,
				window->GetAllocation().Height
			);

			queue->Add(
				Context::Get().GetProjectO().CreateRect(
					shadow_rect,
					shadow_color
				)
			);
		}

		// Pane.
		queue->Add(
			Context::Get().GetProjectO().CreatePane(
				sf::Vector2f( 0.f, 0.f ),
				sf::Vector2f( window->GetAllocation().Width, window->GetAllocation().Height ),
				border_width,
				background_color,
				border_color,
				border_color_shift
			)
		);
	}

	if( window->HasStyle( Window::Resize ) ) {
		queue->Add(
			Context::Get().GetProjectO().CreateTriangle(
				sf::Vector2f( window->GetAllocation().Width, window->GetAllocation().Height - handle_size ),
				sf::Vector2f( window->GetAllocation().Width - handle_size, window->GetAllocation().Height ),
				sf::Vector2f( window->GetAllocation().Width, window->GetAllocation().Height ),
				title_background_color
			)
		);
	}


	if( !window->HasStyle( Window::Titlebar ) ) {
		title_size = 0;
	}

	if( title_size > 0 ) {
		queue->Add(
			Context::Get().GetProjectO().CreateRect(
				sf::FloatRect(
					border_width + .1f,
					border_width + .1f,
					window->GetAllocation().Width - 2 * border_width,
					title_size
				),
				title_background_color
			)
		);

		// Find out visible text, count in "...".
		float avail_width( window->GetAllocation().Width - 2.f * border_width - 2.f * title_padding );

		sf::Text* title_text(
			new sf::Text(
				window->GetTitle(),
				title_font,
				title_font_size
			)
		);

		if( title_text->GetLocalBounds().Width > avail_width ) {
			sf::Text dots( "...", title_font, title_font_size );
			const sf::String& title_string( window->GetTitle() );
			sf::String visible_title;

			avail_width = window->GetAllocation().Width - 2.f * border_width - 2.f * title_padding - dots.GetLocalBounds().Width;

			for( std::size_t ch_index = 0; ch_index < title_string.GetSize(); ++ch_index ) {
				avail_width -= static_cast<float>( title_font.GetGlyph( title_string[ch_index], title_font_size, false ).Advance );

				if( avail_width < 0.f ) {
					visible_title += "...";
					break;
				}

				visible_title += title_string[ch_index];
			}

			title_text->SetString( visible_title );
		}

		// Calculate title text position.
		sf::Vector2f title_position(
			border_width + title_padding,
			border_width + title_size / 2.f - static_cast<float>( title_font_size ) / 2.f
		);

		title_text->SetPosition( title_position );
		title_text->SetColor( title_text_color );

		queue->Add( Context::Get().GetProjectO().CreateText( *title_text ) );

		delete title_text;
	}

	return queue;
}

}
}
