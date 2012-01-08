#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Separator.hpp>

namespace sfg {
namespace eng {

RenderQueue* BREW::CreateSeparatorDrawable( SharedPtr<const Separator> separator ) const {
	sf::Color color( GetProperty<sf::Color>( "Color", separator ) );

	RenderQueue* queue( new RenderQueue );

	if( separator->GetOrientation() == Separator::HORIZONTAL ) {
		queue->Add(
			Context::Get().GetProjectO().CreateLine(
				sf::Vector2f( 0.f, separator->GetAllocation().Height / 2.f ),
				sf::Vector2f( separator->GetAllocation().Width, separator->GetAllocation().Height / 2.f ),
				color,
				1.f
			)
		);
	}
	else {
		queue->Add(
			Context::Get().GetProjectO().CreateLine(
				sf::Vector2f( separator->GetAllocation().Width / 2.f, 0.f ),
				sf::Vector2f( separator->GetAllocation().Width / 2.f, separator->GetAllocation().Height ),
				color,
				1.f
			)
		);
	}

	return queue;
}

}
}
