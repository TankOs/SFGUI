#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/RenderQueue.hpp>
#include <SFGUI/ProgressBar.hpp>

#include <cmath>

namespace sfg {
namespace eng {

std::unique_ptr<RenderQueue> Bob::CreateProgressBarDrawable( std::shared_ptr<const ProgressBar> progress_bar ) const {
	std::unique_ptr<RenderQueue> queue( new RenderQueue );

	//Background
	queue->Add( CreateSpritebox( sf::FloatRect( 0.f, 0.f, progress_bar->GetAllocation().width, progress_bar->GetAllocation().height ),
								 GetResourceManager().GetImage( GetProperty<std::string>( "BackgroundImage", progress_bar ) ),
								 GetProperty<UintRect>( "BackgroundSubRect", progress_bar ) ) );

	//Bar
	if( progress_bar->GetFraction() > 0.f ) {
		float padding = GetProperty<float>( "BarPadding", progress_bar );

		sf::Vector2f bar_dimension(
			std::max( 0.f, progress_bar->GetAllocation().width  - 2.f * padding ),
			std::max( 0.f, progress_bar->GetAllocation().height - 2.f * padding )
		);
		sf::Vector2f bar_position( padding, padding );

		if( progress_bar->GetOrientation() == ProgressBar::Orientation::HORIZONTAL ) {
			bar_dimension.x *= progress_bar->GetFraction();
		}
		else {
			bar_dimension.y *= progress_bar->GetFraction();
			bar_position.y = std::floor( progress_bar->GetAllocation().height + 0.5f) - std::floor( bar_dimension.y + 0.5f ) - padding;
		}

		queue->Add( CreateSpritebox( sf::FloatRect( bar_position, bar_dimension ),
									 GetResourceManager().GetImage( GetProperty<std::string>( "BarImage", progress_bar ) ),
									 GetProperty<UintRect>( "BarSubRect", progress_bar ) ) );
	}

	return queue;
}

}
}
