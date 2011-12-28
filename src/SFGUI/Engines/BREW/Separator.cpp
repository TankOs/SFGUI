#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Separator.hpp>

namespace sfg {
namespace eng {

RenderQueue* BREW::CreateSeparatorDrawable( SharedPtr<const Separator> separator ) const {
	sf::Color color_light( GetProperty<sf::Color>( "Color", separator ) );
	sf::Color color_dark( color_light );
	int color_shift( GetProperty<int>( "BorderColorShift", separator ) );

	ShiftBorderColors( color_light, color_dark, color_shift );

	RenderQueue* queue( new RenderQueue );

	if( separator->GetOrientation() == Separator::HORIZONTAL ) {
		queue->Add(
			CreateLine(
				sf::Vector2f( .5f, std::floor( separator->GetAllocation().Height / 2.f - .5f ) + .5f ),
				sf::Vector2f( std::floor( separator->GetAllocation().Width - .5f ) + .5f, std::floor( separator->GetAllocation().Height / 2.f - .5f ) + .5f ),
				color_dark,
				1.f
			)
		);

		queue->Add(
			CreateLine(
				sf::Vector2f( .5f, std::floor( separator->GetAllocation().Height / 2.f - .5f ) + 1.5f ),
				sf::Vector2f( std::floor( separator->GetAllocation().Width - .5f ) + .5f, std::floor( separator->GetAllocation().Height / 2.f - .5f ) + 1.5f ),
				color_light,
				1.f
			)
		);
	}
	else {
		queue->Add(
			CreateLine(
				sf::Vector2f( std::floor( separator->GetAllocation().Width / 2.f - .5f ) + .5f, .5f ),
				sf::Vector2f( std::floor( separator->GetAllocation().Width / 2.f - .5f ) + .5f, std::floor( separator->GetAllocation().Height - .5f ) + .5f ),
				color_dark,
				1.f
			)
		);

		queue->Add(
			CreateLine(
				sf::Vector2f( std::floor( separator->GetAllocation().Width / 2.f - .5f ) + 1.5f, .5f ),
				sf::Vector2f( std::floor( separator->GetAllocation().Width / 2.f - .5f ) + 1.5f, std::floor( separator->GetAllocation().Height - .5f ) + .5f ),
				color_light,
				1.f
			)
		);
	}

	return queue;
}

}
}
