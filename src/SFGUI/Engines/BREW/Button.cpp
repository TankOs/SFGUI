#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
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
		Renderer::Get().CreatePane(
			sf::Vector2f( 0.f, 0.f ),
			sf::Vector2f( button->GetAllocation().width, button->GetAllocation().height ),
			border_width,
			background_color,
			border_color,
			border_color_shift
		)
	);

	// Label.
	if( button->GetLabel().getSize() > 0 ) {
		sf::Vector2f metrics = GetTextMetrics( button->GetLabel(), font, font_size );
		metrics.y = GetFontLineHeight( font, font_size );

		sf::Text text( button->GetLabel(), font, font_size );
		float offset = ( button->GetState() == Button::ACTIVE ) ? border_width : 0.f;
		sfg::Widget::PtrConst child( button->GetChild() );

		if( !child ) {
			text.setPosition(
				button->GetAllocation().width / 2.f - metrics.x / 2.f + offset,
				button->GetAllocation().height / 2.f - metrics.y / 2.f + offset
			);
		}
		else {
			float width( button->GetAllocation().width - spacing - child->GetAllocation().width );

			text.setPosition(
				child->GetAllocation().width + spacing + (width / 2.f - metrics.x / 2.f) + offset,
				button->GetAllocation().height / 2.f - metrics.y / 2.f + offset
			);
		}

		text.setColor( color );
		queue->Add( Renderer::Get().CreateText( text ) );
	}

	return queue;
}

}
}
