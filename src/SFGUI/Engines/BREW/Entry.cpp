#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Entry.hpp>
#include <SFGUI/RenderQueue.hpp>

#include <SFML/Graphics/Text.hpp>

namespace sfg {
namespace eng {

std::unique_ptr<RenderQueue> BREW::CreateEntryDrawable( std::shared_ptr<const Entry> entry ) const {
	auto border_color = GetProperty<sf::Color>( "BorderColor", entry );
	auto background_color = GetProperty<sf::Color>( "BackgroundColor", entry );
	auto text_color = GetProperty<sf::Color>( "Color", entry );
	auto cursor_color = GetProperty<sf::Color>( "Color", entry );
	auto text_padding = GetProperty<float>( "Padding", entry );
	auto cursor_thickness = GetProperty<float>( "Thickness", entry );
	auto border_width = GetProperty<float>( "BorderWidth", entry );
	auto border_color_shift = GetProperty<int>( "BorderColorShift", entry );
	const auto& font_name = GetProperty<std::string>( "FontName", entry );
	const auto& font = GetResourceManager().GetFont( font_name );
	auto font_size = GetProperty<unsigned int>( "FontSize", entry );

	std::unique_ptr<RenderQueue> queue( new RenderQueue );

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

	auto line_height = GetFontLineHeight( *font, font_size );
	sf::Text vis_label( entry->GetVisibleText(), *font, font_size );
	vis_label.setColor( text_color );
	vis_label.setPosition( text_padding, entry->GetAllocation().height / 2.f - line_height / 2.f );

	queue->Add( Renderer::Get().CreateText( vis_label ) );

	// Draw cursor if entry is active and cursor is visible.
	if( entry->HasFocus() && entry->IsCursorVisible() ) {
		sf::String cursor_string( entry->GetVisibleText() );
		if( entry->GetCursorPosition() - entry->GetVisibleOffset() < static_cast<int>( cursor_string.getSize() ) ) {
			cursor_string.erase( static_cast<std::size_t>( entry->GetCursorPosition() - entry->GetVisibleOffset() ), cursor_string.getSize() );
		}

		// Get metrics.
		sf::Vector2f metrics( GetTextStringMetrics( cursor_string, *font, font_size ) );

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
