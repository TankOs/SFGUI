#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Button.hpp>

#include <SFML/Graphics/Text.hpp>

namespace sfg {
namespace eng {

RenderQueue* BREW::CreateButtonDrawable( SharedPtr<const Button> button ) const {
	sf::Color border_color( GetProperty<sf::Color>( "BorderColor", button ) );
	int border_color_shift( GetProperty<int>( "BorderColorShift", button ) );
	sf::Color background_color( GetProperty<sf::Color>( "BackgroundColor", button ) );
	sf::Color color( GetProperty<sf::Color>( "Color", button ) );
	float border_width( GetProperty<float>( "BorderWidth", button ) );
	float spacing( GetProperty<float>( "Spacing", button ) );
	const std::string& font_name( GetProperty<std::string>( "FontName", button ) );
	unsigned int font_size( GetProperty<unsigned int>( "FontSize", button ) );
	const sf::Font& font( *GetResourceManager().GetFont( font_name ) );

	if( button->GetState() == Button::ACTIVE ) {
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
		float offset = ( button->GetState() == Button::ACTIVE ) ? border_width : 0.f;
		sfg::Widget::PtrConst child( button->GetChild() );

		if( !child ) {
			text->SetPosition(
				button->GetAllocation().Width / 2.f - metrics.x / 2.f + offset,
				button->GetAllocation().Height / 2.f - metrics.y / 2.f + offset
			);
		}
		else {
			float width( button->GetAllocation().Width - spacing - child->GetAllocation().Width );

			text->SetPosition(
				child->GetAllocation().Width + spacing + (width / 2.f - metrics.x / 2.f) + offset,
				button->GetAllocation().Height / 2.f - metrics.y / 2.f + offset
			);
		}

		text->SetColor( color );
		queue->Add( Context::Get().GetProjectO().CreateText( *text ) );

		delete text;
	}

	return queue;
}

}
}
