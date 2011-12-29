#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Separator.hpp>
#include <cmath>

namespace sfg {
namespace eng {

RenderQueue* BREW::CreateSeparatorDrawable( SharedPtr<const Separator> separator ) const {
	sf::Color color( GetProperty<sf::Color>( "Color", separator ) );

	RenderQueue* queue( new RenderQueue );

	if( separator->GetOrientation() == Separator::HORIZONTAL ) {
		queue->Add(
			CreateLine(
				TOP,
				sf::Vector2f( .5f, std::floor( separator->GetAllocation().Height / 2.f - .5f ) + .5f ),
				sf::Vector2f( std::floor( separator->GetAllocation().Width - .5f ) + .5f, std::floor( separator->GetAllocation().Height / 2.f - .5f ) + .5f ),
				color,
				1.f
			)
		);
	}
	else {
		queue->Add(
			CreateLine(
				LEFT,
				sf::Vector2f( std::floor( separator->GetAllocation().Width / 2.f - .5f ) + .5f, .5f ),
				sf::Vector2f( std::floor( separator->GetAllocation().Width / 2.f - .5f ) + .5f, std::floor( separator->GetAllocation().Height - .5f ) + .5f ),
				color,
				1.f
			)
		);
	}

	return queue;
}

}
}
