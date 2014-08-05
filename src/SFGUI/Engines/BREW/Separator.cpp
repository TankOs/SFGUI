#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Separator.hpp>
#include <SFGUI/RenderQueue.hpp>

namespace sfg {
namespace eng {

std::unique_ptr<RenderQueue> BREW::CreateSeparatorDrawable( std::shared_ptr<const Separator> separator ) const {
	auto color = GetProperty<sf::Color>( "Color", separator );

	std::unique_ptr<RenderQueue> queue( new RenderQueue );

	if( separator->GetOrientation() == Separator::Orientation::HORIZONTAL ) {
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
