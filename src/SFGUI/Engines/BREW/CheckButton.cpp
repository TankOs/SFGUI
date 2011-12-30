#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/CheckButton.hpp>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <cmath>

namespace sfg {
namespace eng {

RenderQueue* BREW::CreateCheckButtonDrawable( SharedPtr<const CheckButton> check ) const {
	sf::Color border_color_light( GetProperty<sf::Color>( "BorderColor", check ) );
	sf::Color border_color_dark( border_color_light );
	sf::Color background_color( GetProperty<sf::Color>( "BackgroundColor", check ) );
	sf::Color color( GetProperty<sf::Color>( "Color", check ) );
	sf::Color check_color( GetProperty<sf::Color>( "CheckColor", check ) );
	int border_color_shift( GetProperty<int>( "BorderColorShift", check ) );
	float border_width( GetProperty<float>( "BorderWidth", check ) );
	float box_size( GetProperty<float>( "BoxSize", check ) );
	float spacing( GetProperty<float>( "Spacing", check ) );
	float check_size( std::min( box_size, GetProperty<float>( "CheckSize", check ) ) );
	const std::string& font_name( GetProperty<std::string>( "FontName", check ) );
	unsigned int font_size( GetProperty<unsigned int>( "FontSize", check ) );
	const sf::Font& font( *GetResourceManager().GetFont( font_name ) );
	RenderQueue* queue( new RenderQueue );

	ShiftBorderColors( border_color_light, border_color_dark, border_color_shift );

	// Background.
	sf::RectangleShape* bg_shape = new sf::RectangleShape( sf::Vector2f( box_size, box_size ) );
	bg_shape->SetOutlineColor( sf::Color::Transparent );
	bg_shape->SetFillColor( background_color );
	bg_shape->SetPosition( 0.f, check->GetAllocation().Height / 2.f - box_size / 2.f );
	queue->Add( bg_shape );

	if( check->IsActive() ) {
		float diff( box_size - check_size );

		sf::RectangleShape* check_bg_shape = new sf::RectangleShape(
			sf::Vector2f( check_size - 1.f, check_size - 1.f )
		);
		check_bg_shape->SetOutlineColor( sf::Color::Transparent );
		check_bg_shape->SetFillColor( check_color );
		check_bg_shape->SetPosition( box_size / 2 - check_size / 2, check->GetAllocation().Height / 2.f - box_size / 2.f + diff / 2.f );
		queue->Add( check_bg_shape );
	}

	queue->Add(
		CreateBorder(
			sf::FloatRect(
				0.f,
				check->GetAllocation().Height / 2.f - box_size / 2.f,
				box_size,
				box_size
			),
			border_width,
			border_color_dark,
			border_color_light
		)
	);

	// Label.
	if( check->GetLabel().GetSize() > 0 ) {
		sf::Vector2f metrics = GetTextMetrics( check->GetLabel(), font, font_size );
		metrics.y = GetLineHeight( font, font_size );

		sf::Text* text( new sf::Text( check->GetLabel(), font, font_size ) );
		text->SetPosition(
			std::floor( box_size + spacing ),
			std::floor( check->GetAllocation().Height / 2.f - metrics.y / 2.f + .5f )
		);
		text->SetColor( color );
		queue->Add( text );
	}

	return queue;
}

}
}
