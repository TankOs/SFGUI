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
			sf::Vector2f( progress_bar->GetAllocation().width, progress_bar->GetAllocation().height ),
			border_width,
			background_color,
			border_color,
			-border_color_shift
		)
	);

	if( progress_bar->GetFraction() > 0.f ) {
		sf::FloatRect bar_rect;

		if( progress_bar->GetOrientation() == ProgressBar::Orientation::HORIZONTAL ) {
			float frac_width( std::max( 2.f * bar_border_width, progress_bar->GetAllocation().width * progress_bar->GetFraction() ) );

			bar_rect = sf::FloatRect(
				border_width,
				border_width,
				std::max( 0.f, frac_width - 2.f * border_width ),
				std::max( 0.f, progress_bar->GetAllocation().height - 2.f * border_width )
			);
		}
		else {
			float frac_height( std::max( 2.f * bar_border_width, progress_bar->GetAllocation().height * progress_bar->GetFraction() ) );

			bar_rect = sf::FloatRect(
				border_width,
				std::max( 0.f, progress_bar->GetAllocation().height - frac_height + border_width ),
				std::max( 0.f, progress_bar->GetAllocation().width - 2.f * border_width ),
				std::max( 0.f, frac_height - 2.f * border_width )
			);
		}

		// Bar Pane.
		queue->Add(
			Renderer::Get().CreatePane(
				sf::Vector2f( bar_rect.left, bar_rect.top ),
				sf::Vector2f( bar_rect.width, bar_rect.height ),
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
