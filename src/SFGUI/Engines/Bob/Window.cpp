#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/Window.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/RenderQueue.hpp>
#include <SFGUI/Context.hpp>

#include <SFML/Graphics/Text.hpp>

namespace sfg {
namespace eng {

std::unique_ptr<RenderQueue> Bob::CreateWindowDrawable( std::shared_ptr<const Window> window ) const {
	std::unique_ptr<RenderQueue> queue( new RenderQueue );

	sf::Color background_color( GetProperty<sf::Color>( "BackgroundColor", window ) );
	sf::Color title_text_color( GetProperty<sf::Color>( "Color", window ) );
	float title_padding( GetProperty<float>( "TitlePadding", window ) );
	unsigned int title_font_size( GetProperty<unsigned int>( "FontSize", window ) );
	const sf::Font& title_font( *GetResourceManager().GetFont( GetProperty<std::string>( "FontName", window ) ) );
	float title_size( GetFontLineHeight( title_font, title_font_size ) + 2 * title_padding );

	if( !window->HasStyle( Window::TITLEBAR ) ) {
		title_size = 0.f;
	}

	if( window->HasStyle( Window::BACKGROUND ) ) {
		std::string area_image_property("Image");
		if( title_size > 0 ){
			area_image_property = "AreaImage";
		}

		queue->Add( CreateSpritebox( sf::FloatRect( 0.f, title_size, window->GetAllocation().width, window->GetAllocation().height - title_size ),
									 GetResourceManager().GetImage( GetProperty<std::string>( area_image_property, window ) ),
									 GetProperty<UintRect>( "AreaSubRect", window ) ) );

	}

	if( window->HasStyle( Window::RESIZE ) ) {
		auto handle_image( GetResourceManager().GetImage( GetProperty<std::string>( "HandleImage", window ) ) );
		auto handle_texture( m_texture_manager.GetTexture( handle_image ) );
		if( handle_image ){
			queue->Add(
				Renderer::Get().CreateSprite(
					sf::FloatRect( window->GetAllocation().width  - static_cast<float>( handle_image->getSize().x ),
								   window->GetAllocation().height - static_cast<float>( handle_image->getSize().y ),
					               static_cast<float>( handle_image->getSize().x ),
								   static_cast<float>( handle_image->getSize().y )
					),
					handle_texture
				)
			);
		}
	}

	if( title_size > 0 ) {
		queue->Add( CreateSpritebox( sf::FloatRect( 0.f, 0.f, window->GetAllocation().width, title_size ),
									 GetResourceManager().GetImage( GetProperty<std::string>( "TitleBarImage", window ) ),
									 GetProperty<UintRect>( "TitleBarSubRect", window ) ) );

		// Find out visible text, count in "...".
		float avail_width( window->GetAllocation().width - 2.f * title_padding );

		sf::Text title_text( window->GetTitle(), title_font, title_font_size );

		if( title_text.getLocalBounds().width > avail_width ) {
			sf::Text dots( "...", title_font, title_font_size );
			const sf::String& title_string( window->GetTitle() );
			sf::String visible_title;

			avail_width = window->GetAllocation().width - 2.f * title_padding - dots.getLocalBounds().width;

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
			title_padding,
			title_size / 2.f - static_cast<float>( title_font_size ) / 2.f
		);

		title_text.setPosition( title_position );
		title_text.setColor( title_text_color );

		queue->Add( Renderer::Get().CreateText( title_text ) );
	}

	return queue;
}

}
}
