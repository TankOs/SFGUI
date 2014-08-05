#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/CheckButton.hpp>
#include <SFGUI/RenderQueue.hpp>

#include <SFML/Graphics/Text.hpp>

namespace sfg {
namespace eng {

std::unique_ptr<RenderQueue> BREW::CreateCheckButtonDrawable( std::shared_ptr<const CheckButton> check ) const {
	auto border_color = GetProperty<sf::Color>( "BorderColor", check );
	auto background_color = GetProperty<sf::Color>( "BackgroundColor", check );
	auto color = GetProperty<sf::Color>( "Color", check );
	auto check_color = GetProperty<sf::Color>( "CheckColor", check );
	auto border_color_shift = GetProperty<int>( "BorderColorShift", check );
	auto border_width = GetProperty<float>( "BorderWidth", check );
	auto box_size = GetProperty<float>( "BoxSize", check );
	auto spacing = GetProperty<float>( "Spacing", check );
	auto check_size = std::min( box_size, GetProperty<float>( "CheckSize", check ) );
	const auto& font_name = GetProperty<std::string>( "FontName", check );
	auto font_size = GetProperty<unsigned int>( "FontSize", check );
	const auto& font = GetResourceManager().GetFont( font_name );

	std::unique_ptr<RenderQueue> queue( new RenderQueue );

	// Check Pane.
	queue->Add(
		Renderer::Get().CreatePane(
			sf::Vector2f( 0.f, check->GetAllocation().height / 2.f - box_size / 2.f ),
			sf::Vector2f( box_size, box_size ),
			border_width,
			background_color,
			border_color,
			-border_color_shift
		)
	);

	if( check->IsActive() ) {
		float diff( box_size - check_size );

		queue->Add(
			Renderer::Get().CreateRect(
				sf::FloatRect(
					box_size / 2 - check_size / 2,
					check->GetAllocation().height / 2.f - box_size / 2.f + diff / 2.f,
					check_size - 1.f,
					check_size - 1.f
				),
				check_color
			)
		);
	}

	// Label.
	if( check->GetLabel().getSize() > 0 ) {
		auto metrics = GetTextStringMetrics( check->GetLabel(), *font, font_size );
		metrics.y = GetFontLineHeight( *font, font_size );

		sf::Text text( check->GetLabel(), *font, font_size );
		text.setPosition(
			box_size + spacing,
			check->GetAllocation().height / 2.f - metrics.y / 2.f
		);
		text.setColor( color );
		queue->Add( Renderer::Get().CreateText( text ) );
	}

	return queue;
}

}
}
