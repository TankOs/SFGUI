#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Separator.hpp>

namespace sfg {
namespace eng {

RenderQueue* BREW::CreateSeparatorDrawable( SharedPtr<const Separator> separator ) const {
	sf::Color color( GetProperty<sf::Color>( "Color", separator ) );

	RenderQueue* queue( new RenderQueue );

	if( separator->GetOrientation() == Separator::HORIZONTAL ) {
		queue->Add(
			Renderer::Get().CreateLine(
				sf::Vector2f( 0.f, separator->GetAllocation().height / 2.f ),
				sf::Vector2f( separator->GetAllocation().width, separator->GetAllocation().height / 2.f ),
				color,
				1.f
			)
		);
	}
	else {
		queue->Add(
			Renderer::Get().CreateLine(
				sf::Vector2f( separator->GetAllocation().width / 2.f, 0.f ),
				sf::Vector2f( separator->GetAllocation().width / 2.f, separator->GetAllocation().height ),
				color,
				1.f
			)
		);
	}

	return queue;
}

}
}
