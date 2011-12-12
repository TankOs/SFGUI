#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/ToggleButton.hpp>

namespace sfg {
namespace eng {

RenderQueue* BREW::CreateToggleButtonDrawable( SharedPtr<const ToggleButton> button ) const {
	sf::Color border_color_light( GetProperty<sf::Color>( "BorderColor", button ) );
	sf::Color border_color_dark( border_color_light );
	int border_color_shift( GetProperty<int>( "BorderColorShift", button ) );
	sf::Color background_color( GetProperty<sf::Color>( "BackgroundColor", button ) );
	sf::Color color( GetProperty<sf::Color>( "Color", button ) );
	float border_width( GetProperty<float>( "BorderWidth", button ) );
	const std::string& font_name( GetProperty<std::string>( "FontName", button ) );
	unsigned int font_size( GetProperty<unsigned int>( "FontSize", button ) );
	const sf::Font& font( *GetResourceManager().GetFont( font_name ) );

	ShiftBorderColors( border_color_light, border_color_dark, border_color_shift );

	RenderQueue* queue( new RenderQueue );

	queue->Add(
		new sf::Shape(
			sf::Shape::Rectangle(
				0.f,
				0.f,
				button->GetAllocation().Width,
				button->GetAllocation().Height,
				background_color
			)
		)
	);

	if( button->GetState() != Button::ACTIVE && !button->IsActive() ) {
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
		float offset = ( ( button->GetState() == Button::ACTIVE ) || button->IsActive() ) ? border_width : 0.f;

		text->SetPosition(
			std::floor( button->GetAllocation().Width / 2.f - metrics.x / 2.f + .5f + offset ),
			std::floor( button->GetAllocation().Height / 2.f - metrics.y / 2.f + .5f + offset )
		);
		text->SetColor( color );
		queue->Add( text );
	}

	return queue;
}

}
}
