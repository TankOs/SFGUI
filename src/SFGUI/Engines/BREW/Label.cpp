#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Label.hpp>

namespace sfg {
namespace eng {

RenderQueue* BREW::CreateLabelDrawable( SharedPtr<const Label> label ) const {
	const sf::Font& font( *GetResourceManager().GetFont( GetProperty<std::string>( "FontName", label ) ) );
	const unsigned int font_size( GetProperty<unsigned int>( "FontSize", label ) );
	const sf::Color font_color( GetProperty<sf::Color>( "Color", label ) );

	sf::Text* vis_label( new sf::Text( label->GetText(), font, font_size ) );
	vis_label->SetColor( font_color );

	// Calculate alignment.
	sf::Vector2f avail_space( label->GetAllocation().Width - label->GetRequisition().x, label->GetAllocation().Height - label->GetRequisition().y );
	sf::Vector2f position( avail_space.x * label->GetAlignment().x, avail_space.y * label->GetAlignment().y );

	vis_label->SetPosition( std::floor( position.x + .5f ), std::floor( position.y + .5f ) );

	RenderQueue* queue( new RenderQueue );
	queue->Add( vis_label );

	return queue;
}

}
}
