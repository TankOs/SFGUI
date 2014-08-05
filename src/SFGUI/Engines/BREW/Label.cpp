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
		sf::Vector2f avail_space( label->GetAllocation().width - label->GetRequisition().x, label->GetAllocation().height - label->GetRequisition().y );
		sf::Vector2f position( avail_space.x * label->GetAlignment().x, avail_space.y * label->GetAlignment().y );

		vis_label.setPosition( position.x, position.y );
	}

	queue->Add( Renderer::Get().CreateText( vis_label ) );

	return queue;
}

}
}
