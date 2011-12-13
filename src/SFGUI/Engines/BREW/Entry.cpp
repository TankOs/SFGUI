#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Entry.hpp>

namespace sfg {
namespace eng {

RenderQueue* BREW::CreateEntryDrawable( SharedPtr<const Entry> entry ) const {
	sf::Color border_color_light( GetProperty<sf::Color>( "BorderColor", entry ) );
	sf::Color border_color_dark( GetProperty<sf::Color>( "BorderColor", entry ) );
	sf::Color background_color( GetProperty<sf::Color>( "BackgroundColor", entry ) );
	sf::Color text_color( GetProperty<sf::Color>( "Color", entry ) );
	sf::Color cursor_color( GetProperty<sf::Color>( "Color", entry ) );
	float text_padding( GetProperty<float>( "Padding", entry ) );
	float cursor_thickness( GetProperty<float>( "Thickness", entry ) );
	float border_width( GetProperty<float>( "BorderWidth", entry ) );
	int border_color_shift( GetProperty<int>( "BorderColorShift", entry ) );
	const sf::Font& font( *GetResourceManager().GetFont( GetProperty<std::string>( "FontName", entry ) ) );
	const unsigned int& font_size( GetProperty<unsigned int>( "FontSize", entry ) );

	ShiftBorderColors( border_color_light, border_color_dark, border_color_shift );

	RenderQueue* queue( new RenderQueue );

	queue->Add(
		new sf::Shape(
			sf::Shape::Rectangle(
				0.f, 0.f,
				entry->GetAllocation().Width,
				entry->GetAllocation().Height,
				background_color
			)
		)
	);

	queue->Add( CreateBorder( sf::FloatRect( 0.f, 0.f, entry->GetAllocation().Width, entry->GetAllocation().Height ), border_width, border_color_dark, border_color_light) );

	float line_height = GetLineHeight( font, font_size );
	sf::Text* vis_label( new sf::Text( entry->GetVisibleText(), font, font_size ) );
	vis_label->SetColor( text_color );
	vis_label->SetPosition( std::floor( text_padding + .5f ), std::floor( entry->GetAllocation().Height / 2.f - line_height / 2.f + .5f ) );

	queue->Add( vis_label );

	// Draw cursor if entry is active and cursor is visible.
	if( entry->GetState() == Widget::ACTIVE && entry->IsCursorVisible() ) {
		sf::String cursor_string( entry->GetVisibleText() );
		if( entry->GetCursorPosition() - entry->GetVisibleOffset() < cursor_string.GetSize() ) {
			cursor_string.Erase( entry->GetCursorPosition() - entry->GetVisibleOffset(), cursor_string.GetSize() );
		}

		// Get metrics.
		sf::Vector2f metrics( GetTextMetrics( cursor_string, font, font_size ) );

		sf::Sprite* vis_cursor( new sf::Sprite() );
		vis_cursor->SetPosition( metrics.x + text_padding, entry->GetAllocation().Height / 2.f - line_height / 2.f );
		vis_cursor->Resize( cursor_thickness, line_height );
		vis_cursor->SetColor( cursor_color );
		queue->Add( vis_cursor );
	}

	return queue;
}

}
}
