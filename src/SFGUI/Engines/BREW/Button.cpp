#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Button.hpp>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <cmath>

namespace sfg {
namespace eng {

RenderQueue* BREW::CreateButtonDrawable( SharedPtr<const Button> button ) const {
	sf::Color border_color_light( GetProperty<sf::Color>( "BorderColor", button ) );
	sf::Color border_color_dark( border_color_light );
	int border_color_shift( GetProperty<int>( "BorderColorShift", button ) );
	sf::Color background_color( GetProperty<sf::Color>( "BackgroundColor", button ) );
	sf::Color color( GetProperty<sf::Color>( "Color", button ) );
	float border_width( GetProperty<float>( "BorderWidth", button ) );
	float spacing( GetProperty<float>( "Spacing", button ) );
	const std::string& font_name( GetProperty<std::string>( "FontName", button ) );
	unsigned int font_size( GetProperty<unsigned int>( "FontSize", button ) );
	const sf::Font& font( *GetResourceManager().GetFont( font_name ) );

	ShiftBorderColors( border_color_light, border_color_dark, border_color_shift );

	RenderQueue* queue( new RenderQueue );

	// Background.
	sf::RectangleShape* rect_shape = new sf::RectangleShape(
		sf::Vector2f( button->GetAllocation().Width, button->GetAllocation().Height )
	);
	rect_shape->SetOutlineColor( sf::Color::Transparent );
	rect_shape->SetFillColor( background_color );

	queue->Add( rect_shape );

	// Border.
	if( button->GetState() != Button::ACTIVE ) {
		queue->Add( CreateBorder( sf::FloatRect( 0.f, 0.f, button->GetAllocation().Width, button->GetAllocation().Height ), border_width, border_color_light, border_color_dark ) );
	}
	else {
		queue->Add( CreateBorder( sf::FloatRect( 0.f, 0.f, button->GetAllocation().Width, button->GetAllocation().Height ), border_width, border_color_dark, border_color_light ) );
	}

	// Label.
	if( button->GetLabel().GetSize() > 0 ) {
		sf::Vector2f metrics = GetTextMetrics( button->GetLabel(), font, font_size );
		metrics.y = GetLineHeight( font, font_size );

		sf::Text* text( new sf::Text( button->GetLabel(), font, font_size ) );
		float offset = ( button->GetState() == Button::ACTIVE ) ? border_width : 0.f;
		sfg::Widget::PtrConst child( button->GetChild() );

		if( !child ) {
			text->SetPosition(
				std::floor( button->GetAllocation().Width / 2.f - metrics.x / 2.f + .5f + offset ),
				std::floor( button->GetAllocation().Height / 2.f - metrics.y / 2.f + .5f + offset )
			);
		}
		else {
			float width( button->GetAllocation().Width - spacing - child->GetAllocation().Width );

			text->SetPosition(
				std::floor( child->GetAllocation().Width + spacing + (width / 2.f - metrics.x / 2.f) + .5f + offset ),
				std::floor( button->GetAllocation().Height / 2.f - metrics.y / 2.f + .5f + offset )
			);
		}

		text->SetColor( color );
		queue->Add( text );
	}

	return queue;
}

}
}
