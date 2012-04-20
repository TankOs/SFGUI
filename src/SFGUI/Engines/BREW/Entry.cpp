#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Entry.hpp>

#include <SFML/Graphics/Text.hpp>

namespace sfg {
namespace eng {

RenderQueue* BREW::CreateEntryDrawable( SharedPtr<const Entry> entry ) const {
	sf::Color border_color( GetProperty<sf::Color>( "BorderColor", entry ) );
	sf::Color background_color( GetProperty<sf::Color>( "BackgroundColor", entry ) );
	sf::Color text_color( GetProperty<sf::Color>( "Color", entry ) );
	sf::Color cursor_color( GetProperty<sf::Color>( "Color", entry ) );
	float text_padding( GetProperty<float>( "Padding", entry ) );
	float cursor_thickness( GetProperty<float>( "Thickness", entry ) );
	float border_width( GetProperty<float>( "BorderWidth", entry ) );
	int border_color_shift( GetProperty<int>( "BorderColorShift", entry ) );
	const sf::Font& font( *GetResourceManager().GetFont( GetProperty<std::string>( "FontName", entry ) ) );
	const unsigned int& font_size( GetProperty<unsigned int>( "FontSize", entry ) );

	RenderQueue* queue( new RenderQueue );

	// Pane.
	queue->Add(
		Renderer::Get().CreatePane(
			sf::Vector2f( 0.f, 0.f ),
			sf::Vector2f( entry->GetAllocation().width, entry->GetAllocation().height ),
			border_width,
			background_color,
			border_color,
			-border_color_shift
		)
	);

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
