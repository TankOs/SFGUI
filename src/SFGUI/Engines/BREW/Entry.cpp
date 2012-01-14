#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Context.hpp>
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
		Context::Get().GetProjectO().CreatePane(
			sf::Vector2f( 0.f, 0.f ),
			sf::Vector2f( entry->GetAllocation().Width, entry->GetAllocation().Height ),
			border_width,
			background_color,
			border_color,
			-border_color_shift
		)
	);

	float line_height = GetLineHeight( font, font_size );
	sf::Text* vis_label( new sf::Text( entry->GetVisibleText(), font, font_size ) );
	vis_label->SetColor( text_color );
	vis_label->SetPosition( text_padding, entry->GetAllocation().Height / 2.f - line_height / 2.f );

	queue->Add( Context::Get().GetProjectO().CreateText( *vis_label ) );

	delete vis_label;

	// Draw cursor if entry is active and cursor is visible.
	if( entry->GetState() == Widget::ACTIVE && entry->IsCursorVisible() ) {
		sf::String cursor_string( entry->GetVisibleText() );
		if( entry->GetCursorPosition() - entry->GetVisibleOffset() < cursor_string.GetSize() ) {
			cursor_string.Erase( entry->GetCursorPosition() - entry->GetVisibleOffset(), cursor_string.GetSize() );
		}

		// Get metrics.
		sf::Vector2f metrics( GetTextMetrics( cursor_string, font, font_size ) );

		queue->Add(
			Context::Get().GetProjectO().CreateRect(
				sf::FloatRect(
					metrics.x + text_padding,
					entry->GetAllocation().Height / 2.f - line_height / 2.f,
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
