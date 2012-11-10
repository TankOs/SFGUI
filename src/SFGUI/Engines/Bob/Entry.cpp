#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Entry.hpp>

#include <SFGUI/Engines/Bob/Spritebox.hpp>

namespace sfg {
namespace eng {

RenderQueue* Bob::CreateEntryDrawable( SharedPtr<const Entry> entry ) const {
	sf::Color text_color( GetProperty<sf::Color>( "Color", entry ) );
	sf::Color cursor_color( GetProperty<sf::Color>( "Color", entry ) );
	float text_padding( GetProperty<float>( "Padding", entry ) );
	float cursor_thickness( GetProperty<float>( "Thickness", entry ) );
	const sf::Font& font( *GetResourceManager().GetFont( GetProperty<std::string>( "FontName", entry ) ) );
	const unsigned int& font_size( GetProperty<unsigned int>( "FontSize", entry ) );

	RenderQueue* queue( new RenderQueue );

	//Spritebox
	bob::Spritebox spritebox;
	spritebox.SetDimension( sf::Vector2i( static_cast<int>( entry->GetAllocation().width ), static_cast<int>( entry->GetAllocation().height ) ) );

	const sf::Image *image( GetResourceManager().GetImage( GetProperty<std::string>( "Image", entry ) ) );
	if( image == NULL )
		return queue;

	SharedPtr< Primitive::Texture > texture_handle( m_texture_manager.GetTexture( image ) );
	spritebox.SetTexture( texture_handle );

	Primitive::Ptr primitive = spritebox.ConstructPrimitive();

	Renderer::Get().AddPrimitive( primitive );
	queue->Add( primitive );

	float line_height = GetFontLineHeight( font, font_size );
	sf::Text vis_label( entry->GetVisibleText(), font, font_size );
	vis_label.setColor( text_color );
	vis_label.setPosition( text_padding, entry->GetAllocation().height / 2.f - line_height / 2.f );

	queue->Add( Renderer::Get().CreateText( vis_label ) );

	// Draw cursor if entry is active and cursor is visible.
	if( entry->HasFocus() && entry->IsCursorVisible() ) {
		sf::String cursor_string( entry->GetVisibleText() );
		if( entry->GetCursorPosition() - entry->GetVisibleOffset() < cursor_string.getSize() ) {
			cursor_string.erase( entry->GetCursorPosition() - entry->GetVisibleOffset(), cursor_string.getSize() );
		}

		// Get metrics.
		sf::Vector2f metrics( GetTextMetrics( cursor_string, font, font_size ) );

		queue->Add(
			Renderer::Get().CreateRect(
				sf::FloatRect(
					metrics.x + text_padding,
					entry->GetAllocation().height / 2.f - line_height / 2.f,
					cursor_thickness,
					line_height
				),
				cursor_color
			)
		);
	}

	return queue;
}

}
}
