#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/ProgressBar.hpp>
#include <cmath>

namespace sfg {
namespace eng {

RenderQueue* BREW::CreateProgressBarDrawable( SharedPtr<const ProgressBar> progress_bar ) const {
	sf::Color border_color( GetProperty<sf::Color>( "BorderColor", progress_bar ) );
	sf::Color bar_border_color( GetProperty<sf::Color>( "BarBorderColor", progress_bar ) );
	sf::Color background_color( GetProperty<sf::Color>( "BackgroundColor", progress_bar ) );
	sf::Color progress_color( GetProperty<sf::Color>( "BarColor", progress_bar ) );
	int border_color_shift( GetProperty<int>( "BorderColorShift", progress_bar ) );
	int bar_border_color_shift( GetProperty<int>( "BarBorderColorShift", progress_bar ) );
	float border_width( GetProperty<float>( "BorderWidth", progress_bar ) );
	float bar_border_width( GetProperty<float>( "BarBorderWidth", progress_bar ) );

	RenderQueue* queue( new RenderQueue );

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

		if( progress_bar->GetOrientation() == ProgressBar::HORIZONTAL ) {
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
