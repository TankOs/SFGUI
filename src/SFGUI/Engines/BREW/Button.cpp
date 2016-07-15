#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/RenderQueue.hpp>

#include <SFML/Graphics/Text.hpp>

namespace sfg {
namespace eng {

std::unique_ptr<RenderQueue> BREW::CreateButtonDrawable( std::shared_ptr<const Button> button ) const {
	auto border_color = GetProperty<sf::Color>( "BorderColor", button );
	auto border_color_shift = GetProperty<int>( "BorderColorShift", button );
	auto background_color = GetProperty<sf::Color>( "BackgroundColor", button );
	auto color = GetProperty<sf::Color>( "Color", button );
	auto border_width = GetProperty<float>( "BorderWidth", button );
	auto spacing = GetProperty<float>( "Spacing", button );
	const auto& font_name = GetProperty<std::string>( "FontName", button );
	auto font_size = GetProperty<unsigned int>( "FontSize", button );
	const auto& font = GetResourceManager().GetFont( font_name );

	if( button->GetState() == Button::State::ACTIVE ) {
		border_color_shift = -border_color_shift;
	}

	std::unique_ptr<RenderQueue> queue( new RenderQueue );

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
		auto metrics = GetTextStringMetrics( button->GetLabel(), *font, font_size );
		metrics.y = GetFontLineHeight( *font, font_size );

		sf::Text text( button->GetLabel(), *font, font_size );
		auto offset = ( button->GetState() == Button::State::ACTIVE ) ? border_width : 0.f;
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

		text.setFillColor( color );
		queue->Add( Renderer::Get().CreateText( text ) );
	}

	return queue;
}

}
}
