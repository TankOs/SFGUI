#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/Window.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engines/Bob/Spritebox.hpp>

#include <SFML/Graphics/Text.hpp>

namespace sfg {
namespace eng {

RenderQueue* Bob::CreateWindowDrawable( SharedPtr<const Window> window ) const {
	RenderQueue* queue( new RenderQueue );

	sf::Color background_color( GetProperty<sf::Color>( "BackgroundColor", window ) );
	sf::Color title_text_color( GetProperty<sf::Color>( "Color", window ) );
	float title_padding( GetProperty<float>( "TitlePadding", window ) );
	unsigned int title_font_size( GetProperty<unsigned int>( "FontSize", window ) );
	const sf::Font& title_font( *GetResourceManager().GetFont( GetProperty<std::string>( "FontName", window ) ) );
	float title_size( GetFontLineHeight( title_font, title_font_size ) + 2 * title_padding );

	if( !window->HasStyle( Window::TITLEBAR ) ) {
		title_size = 0;
	}

	if( window->HasStyle( Window::BACKGROUND ) ) {
		std::string area_image_path;
		if( title_size > 0 ){
			area_image_path = GetProperty<std::string>( "AreaImage", window );
		}
		else {
			area_image_path = GetProperty<std::string>( "Image", window );
		}

		const sf::Image *area_image( GetResourceManager().GetImage( area_image_path ) );
		if( area_image == NULL )
			return queue;

		SharedPtr< Primitive::Texture > texture_handle( sfg::Renderer::Get().LoadImage( *area_image ) );

		if( texture_handle != 0 ){
			bob::Spritebox windowSpritebox;
			windowSpritebox.SetTexture( texture_handle );
			windowSpritebox.SetPosition( sf::Vector2f( 0, title_size ) );
			windowSpritebox.SetDimension( sf::Vector2i( static_cast<int>( window->GetAllocation().width ), static_cast<int>( window->GetAllocation().height - title_size ) ) );

			Primitive::Ptr primitive = windowSpritebox.ConstructPrimitive();

			Renderer::Get().AddPrimitive( primitive );
			queue->Add( primitive );
		}
	}

	if( window->HasStyle( Window::RESIZE ) ) {
		const sf::Image *handle_image( GetResourceManager().GetImage( GetProperty<std::string>( "HandleImage", window ) ) );
		if( handle_image != NULL ){
			queue->Add(
				Renderer::Get().CreateImage(
					sf::FloatRect( window->GetAllocation().width  - handle_image->getSize().x,
					               window->GetAllocation().height - handle_image->getSize().y,
					               handle_image->getSize().x,
								   handle_image->getSize().y
					),
					*handle_image
				)
			);
		}
	}

	if( title_size > 0 ) {
		const sf::Image *titlebar_image( GetResourceManager().GetImage( GetProperty<std::string>( "TitleBarImage", window ) ) );
		if( titlebar_image == NULL )
			return queue;

		SharedPtr< Primitive::Texture > texture_handle = sfg::Renderer::Get().LoadImage( *titlebar_image );

		if(texture_handle != 0){
			bob::Spritebox windowSpritebox;
			windowSpritebox.SetTexture( texture_handle );
			windowSpritebox.SetDimension(sf::Vector2i( static_cast<int>( window->GetAllocation().width ), static_cast<int>( title_size ) ) );

			Primitive::Ptr primitive = windowSpritebox.ConstructPrimitive();

			Renderer::Get().AddPrimitive( primitive );
			queue->Add( primitive );
		}

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
