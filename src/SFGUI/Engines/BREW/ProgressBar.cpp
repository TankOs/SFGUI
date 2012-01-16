#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Context.hpp>
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
		Context::Get().GetRenderer().CreatePane(
			sf::Vector2f( 0.f, 0.f ),
			sf::Vector2f( progress_bar->GetAllocation().Width, progress_bar->GetAllocation().Height ),
			border_width,
			background_color,
			border_color,
			-border_color_shift
		)
	);

	if( progress_bar->GetFraction() > 0.f ) {
		sf::FloatRect bar_rect;

		if( progress_bar->GetOrientation() == ProgressBar::HORIZONTAL ) {
			float frac_width( std::max( 2.f * bar_border_width, progress_bar->GetAllocation().Width * progress_bar->GetFraction() ) );

			bar_rect = sf::FloatRect(
				border_width,
				border_width,
				std::max( 0.f, frac_width - 2.f * border_width ),
				std::max( 0.f, progress_bar->GetAllocation().Height - 2.f * border_width )
			);
		}
		else {
			float frac_height( std::max( 2.f * bar_border_width, progress_bar->GetAllocation().Height * progress_bar->GetFraction() ) );

			bar_rect = sf::FloatRect(
				border_width,
				std::max( 0.f, progress_bar->GetAllocation().Height - frac_height + border_width ),
				std::max( 0.f, progress_bar->GetAllocation().Width - 2.f * border_width ),
				std::max( 0.f, frac_height - 2.f * border_width )
			);
		}

		// Bar Pane.
		queue->Add(
			Context::Get().GetRenderer().CreatePane(
				sf::Vector2f( bar_rect.Left, bar_rect.Top ),
				sf::Vector2f( bar_rect.Width, bar_rect.Height ),
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
