#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Label.hpp>

#include <SFML/Graphics/Text.hpp>

namespace sfg {
namespace eng {

RenderQueue* BREW::CreateLabelDrawable( SharedPtr<const Label> label ) const {
	const sf::Font& font( *GetResourceManager().GetFont( GetProperty<std::string>( "FontName", label ) ) );
	const unsigned int font_size( GetProperty<unsigned int>( "FontSize", label ) );
	const sf::Color font_color( GetProperty<sf::Color>( "Color", label ) );
	const sf::Color background_color( GetProperty<sf::Color>( "BackgroundColor", label ) );

	sf::Text vis_label( label->GetWrappedText(), font, font_size );
	vis_label.setColor( font_color );

	if( !label->GetLineWrap() ) {
		// Calculate alignment when word wrap is disabled.
		sf::Vector2f avail_space( label->GetAllocation().width - label->GetRequisition().x, label->GetAllocation().height - label->GetRequisition().y );
		sf::Vector2f position( avail_space.x * label->GetAlignment().x, avail_space.y * label->GetAlignment().y );

		vis_label.setPosition( position.x, position.y );
	}

	RenderQueue* queue( new RenderQueue );
	queue->Add( Renderer::Get().CreateText( vis_label ) );

	return queue;
}

}
}
