#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/RenderQueue.hpp>

#include <SFML/Graphics/Text.hpp>

namespace sfg {
namespace eng {

std::unique_ptr<RenderQueue> BREW::CreateLabelDrawable( std::shared_ptr<const Label> label ) const {
	const auto& font_name = GetProperty<std::string>( "FontName", label );
	const auto& font = GetResourceManager().GetFont( font_name );
	auto font_size = GetProperty<unsigned int>( "FontSize", label );
	auto font_color = GetProperty<sf::Color>( "Color", label );

	std::unique_ptr<RenderQueue> queue( new RenderQueue );

	sf::Text vis_label( label->GetWrappedText(), *font, font_size );
	vis_label.setColor( font_color );

	if( !label->GetLineWrap() ) {
		// Calculate alignment when word wrap is disabled.
		auto metrics = GetTextStringMetrics( label->GetWrappedText(), *font, font_size );
		metrics.y = GetFontLineHeight( *font, font_size );

		Misc::Alignment nAlignment = label->GetAlignment();
		sf::Vector2f nPosition;
		nPosition.x = label->GetAllocation().width / nAlignment.position.x;
		nPosition.y = label->GetAllocation().height / nAlignment.position.y - metrics.y / 2.f;

		switch ( nAlignment.justification ) {
		case Misc::Justify::LEFT:
			break;
		case Misc::Justify::RIGHT:
			nPosition.x -= metrics.x;
		default:
			nPosition.x += metrics.x / 2.f;
			break;
		}

		vis_label.setPosition( nPosition.x, nPosition.y );
	}

	queue->Add( Renderer::Get().CreateText( vis_label ) );

	return queue;
}

}
}
