#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/ToggleButton.hpp>
#include <SFGUI/RenderQueue.hpp>

#include <SFML/Graphics/Text.hpp>

namespace sfg {
namespace eng {

std::unique_ptr<RenderQueue> BREW::CreateToggleButtonDrawable( std::shared_ptr<const ToggleButton> button ) const {
	auto border_color = GetProperty<sf::Color>( "BorderColor", button );
	auto border_color_shift = GetProperty<int>( "BorderColorShift", button );
	auto background_color = GetProperty<sf::Color>( "BackgroundColor", button );
	auto color = GetProperty<sf::Color>( "Color", button );
	auto border_width = GetProperty<float>( "BorderWidth", button );
	const auto& font_name = GetProperty<std::string>( "FontName", button );
	auto font_size = GetProperty<unsigned int>( "FontSize", button );
	const auto& font = GetResourceManager().GetFont( font_name );

	if( ( button->GetState() == Button::State::ACTIVE ) || button->IsActive() ) {
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
		auto offset = ( ( button->GetState() == Button::State::ACTIVE ) || button->IsActive() ) ? border_width : 0.f;

		Misc::Alignment nAlignment = button->GetAlignment();
		sf::Vector2f nPosition;
		nPosition.x = button->GetAllocation().width * nAlignment.position.x;
		nPosition.y = button->GetAllocation().height * nAlignment.position.y - metrics.y / 2.f + offset;

		switch ( nAlignment.justification ) {
		case Misc::Justify::LEFT:
			nPosition.x += offset;
			break;
		case Misc::Justify::RIGHT:
			nPosition.x -= metrics.x + offset;
			break;
		default:
			nPosition.x -= metrics.x / 2.f + offset;
			break;
		}

		text.setPosition( nPosition.x, nPosition.y );
		text.setColor( color );
		queue->Add( Renderer::Get().CreateText( text ) );
	}

	return queue;
}

}
}
