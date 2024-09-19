#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/ProgressBar.hpp>
#include <SFGUI/RenderQueue.hpp>

#include <algorithm>

namespace sfg {
namespace eng {

std::unique_ptr<RenderQueue> BREW::CreateProgressBarDrawable( std::shared_ptr<const ProgressBar> progress_bar ) const {
	auto border_color = GetProperty<sf::Color>( "BorderColor", progress_bar );
	auto bar_border_color = GetProperty<sf::Color>( "BarBorderColor", progress_bar );
	auto background_color = GetProperty<sf::Color>( "BackgroundColor", progress_bar );
	auto progress_color = GetProperty<sf::Color>( "BarColor", progress_bar );
	auto border_color_shift = GetProperty<int>( "BorderColorShift", progress_bar );
	auto bar_border_color_shift = GetProperty<int>( "BarBorderColorShift", progress_bar );
	auto border_width = GetProperty<float>( "BorderWidth", progress_bar );
	auto bar_border_width = GetProperty<float>( "BarBorderWidth", progress_bar );

	std::unique_ptr<RenderQueue> queue( new RenderQueue );

	// Pane.
	queue->Add(
		Renderer::Get().CreatePane(
			sf::Vector2f( 0.f, 0.f ),
			sf::Vector2f( progress_bar->GetAllocation().size.x, progress_bar->GetAllocation().size.y ),
			border_width,
			background_color,
			border_color,
			-border_color_shift
		)
	);

	if( progress_bar->GetFraction() > 0.f ) {
		sf::FloatRect bar_rect;

		if( progress_bar->GetOrientation() == ProgressBar::Orientation::HORIZONTAL ) {
			float frac_width( std::max( 2.f * bar_border_width, progress_bar->GetAllocation().size.x * progress_bar->GetFraction() ) );

			bar_rect = sf::FloatRect(
				{ border_width, border_width },
				{ std::max( 0.f, frac_width - 2.f * border_width ), std::max( 0.f, progress_bar->GetAllocation().size.y - 2.f * border_width ) }
			);
		}
		else {
			float frac_height( std::max( 2.f * bar_border_width, progress_bar->GetAllocation().size.y * progress_bar->GetFraction() ) );

			bar_rect = sf::FloatRect(
				{ border_width, std::max( 0.f, progress_bar->GetAllocation().size.y - frac_height + border_width ) },
				{ std::max( 0.f, progress_bar->GetAllocation().size.x - 2.f * border_width ), std::max( 0.f, frac_height - 2.f * border_width ) }
			);
		}

		// Bar Pane.
		queue->Add(
			Renderer::Get().CreatePane(
				sf::Vector2f( bar_rect.position.x, bar_rect.position.y ),
				sf::Vector2f( bar_rect.size.x, bar_rect.size.y ),
				bar_border_width,
				progress_color,
				bar_border_color,
				bar_border_color_shift
			)
		);
	}

	return queue;
}

}
}
