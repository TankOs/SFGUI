#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/ToggleButton.hpp>

#include <SFML/Graphics/Text.hpp>

namespace sfg {
namespace eng {

RenderQueue* BREW::CreateToggleButtonDrawable( SharedPtr<const ToggleButton> button ) const {
	sf::Color border_color( GetProperty<sf::Color>( "BorderColor", button ) );
	int border_color_shift( GetProperty<int>( "BorderColorShift", button ) );
	sf::Color background_color( GetProperty<sf::Color>( "BackgroundColor", button ) );
	sf::Color color( GetProperty<sf::Color>( "Color", button ) );
	float border_width( GetProperty<float>( "BorderWidth", button ) );
	const std::string& font_name( GetProperty<std::string>( "FontName", button ) );
	unsigned int font_size( GetProperty<unsigned int>( "FontSize", button ) );
	const sf::Font& font( *GetResourceManager().GetFont( font_name ) );

	if( ( button->GetState() == Button::ACTIVE ) || button->IsActive() ) {
		border_color_shift = -border_color_shift;
	}

	RenderQueue* queue( new RenderQueue );

	// Pane.
	queue->Add(
		Context::Get().GetProjectO().CreatePane(
			sf::Vector2f( 0.f, 0.f ),
			sf::Vector2f( button->GetAllocation().Width, button->GetAllocation().Height ),
			border_width,
			background_color,
			border_color,
			border_color_shift
		)
	);

	// Label.
	if( button->GetLabel().GetSize() > 0 ) {
		sf::Vector2f metrics = GetTextMetrics( button->GetLabel(), font, font_size );
		metrics.y = GetLineHeight( font, font_size );

		sf::Text* text( new sf::Text( button->GetLabel(), font, font_size ) );
		float offset = ( ( button->GetState() == Button::ACTIVE ) || button->IsActive() ) ? border_width : 0.f;

		text->SetPosition(
			button->GetAllocation().Width / 2.f - metrics.x / 2.f + offset,
			button->GetAllocation().Height / 2.f - metrics.y / 2.f + offset
		);
		text->SetColor( color );
		queue->Add( Context::Get().GetProjectO().CreateText( *text ) );

		delete text;
	}

	return queue;
}

}
}
