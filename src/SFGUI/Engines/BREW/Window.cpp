#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Window.hpp>
#include <SFGUI/RenderQueue.hpp>

#include <SFML/Graphics/Text.hpp>

#include <cmath>

namespace sfg {
namespace eng {

std::unique_ptr<RenderQueue> BREW::CreateWindowDrawable( std::shared_ptr<const Window> window ) const {
	auto background_color = GetProperty<sf::Color>( "BackgroundColor", window );
	auto border_color = GetProperty<sf::Color>( "BorderColor", window );
	auto title_background_color = GetProperty<sf::Color>( "TitleBackgroundColor", window );
	auto title_text_color = GetProperty<sf::Color>( "Color", window );
	auto border_color_shift = GetProperty<int>( "BorderColorShift", window );
	auto border_width = GetProperty<float>( "BorderWidth", window );
	auto title_padding = GetProperty<float>( "TitlePadding", window );
	auto shadow_distance = GetProperty<float>( "ShadowDistance", window );
	auto handle_size = GetProperty<float>( "HandleSize", window );
	auto shadow_alpha = GetProperty<sf::Uint8>( "ShadowAlpha", window );
	auto title_font_size = GetProperty<unsigned int>( "FontSize", window );
	auto close_height = GetProperty<float>( "CloseHeight", window );
	auto close_thickness = GetProperty<float>( "CloseThickness", window );
	const auto& title_font_name = GetProperty<std::string>( "FontName", window );
	const auto& title_font = GetResourceManager().GetFont( title_font_name );

	auto title_size = GetFontLineHeight( *title_font, title_font_size ) + 2 * title_padding;

	std::unique_ptr<RenderQueue> queue( new RenderQueue );

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

		if( window->HasStyle( Window::CLOSE ) ) {
			auto button_margin = ( title_size - close_height ) / 2.f;
			auto corner_extent = std::sqrt( ( close_thickness * close_thickness ) / 2.f ) / 2.f;

			queue->Add(
				Renderer::Get().CreateLine(
					sf::Vector2f(
						std::floor( window->GetAllocation().width - border_width - button_margin - close_height + corner_extent + .5f ),
						std::floor( border_width + button_margin + corner_extent + .5f )
					),
					sf::Vector2f(
						std::floor( window->GetAllocation().width - border_width - button_margin - corner_extent + .5f ),
						std::floor( border_width + title_size - button_margin - corner_extent + .5f )
					),
					title_text_color,
					close_thickness
				)
			);

			queue->Add(
				Renderer::Get().CreateLine(
					sf::Vector2f(
						std::floor( window->GetAllocation().width - border_width - button_margin - close_height + corner_extent + .5f ),
						std::floor( border_width + title_size - button_margin - corner_extent + .5f )
					),
					sf::Vector2f(
						std::floor( window->GetAllocation().width - border_width - button_margin - corner_extent + .5f ),
						std::floor( border_width + button_margin + corner_extent + .5f )
					),
					title_text_color,
					close_thickness
				)
			);
		}

		// Find out visible text, count in "...".
		float avail_width( window->GetAllocation().width - 2.f * border_width - 2.f * title_padding - ( window->HasStyle( Window::CLOSE ) ? title_size : 0 ) );

		sf::Text title_text( window->GetTitle(), *title_font, title_font_size );

		if( title_text.getLocalBounds().width > avail_width ) {
			sf::Text dots( "...", *title_font, title_font_size );
			const sf::String& title_string( window->GetTitle() );
			sf::String visible_title;

			avail_width = window->GetAllocation().width - 2.f * border_width - 2.f * title_padding - dots.getLocalBounds().width;

			for( const auto& character : title_string ) {
				avail_width -= static_cast<float>( title_font->getGlyph( character, title_font_size, false ).advance );

				if( avail_width < 0.f ) {
					visible_title += "...";
					break;
				}

				visible_title += character;
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
