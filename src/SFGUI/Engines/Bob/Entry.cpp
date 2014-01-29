#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Entry.hpp>

namespace sfg {
namespace eng {

std::unique_ptr<RenderQueue> Bob::CreateEntryDrawable( std::shared_ptr<const Entry> entry ) const {
	sf::Color text_color( GetProperty<sf::Color>( "Color", entry ) );
	sf::Color cursor_color( GetProperty<sf::Color>( "Color", entry ) );
	float text_padding( GetProperty<float>( "Padding", entry ) );
	float cursor_thickness( GetProperty<float>( "Thickness", entry ) );
	const sf::Font& font( *GetResourceManager().GetFont( GetProperty<std::string>( "FontName", entry ) ) );
	const unsigned int& font_size( GetProperty<unsigned int>( "FontSize", entry ) );

	std::unique_ptr<RenderQueue> queue( new RenderQueue );

	//Spritebox
	queue->Add( CreateSpritebox( sf::FloatRect( 0.f, 0.f, entry->GetAllocation().width, entry->GetAllocation().height ),
								 GetResourceManager().GetImage( GetProperty<std::string>( "Image", entry ) ),
								 GetProperty<UintRect>( "SubRect", entry ) ) );

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
