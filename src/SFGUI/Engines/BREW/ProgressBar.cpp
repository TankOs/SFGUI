#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/ProgressBar.hpp>

namespace sfg {
namespace eng {

RenderQueue* BREW::CreateProgressBarDrawable( SharedPtr<const ProgressBar> progress_bar ) const {
	sf::Color border_color_light( GetProperty<sf::Color>( "BorderColor", progress_bar ) );
	sf::Color border_color_dark( GetProperty<sf::Color>( "BorderColor", progress_bar ) );
	sf::Color bar_border_color_light( GetProperty<sf::Color>( "BarBorderColor", progress_bar ) );
	sf::Color bar_border_color_dark( GetProperty<sf::Color>( "BarBorderColor", progress_bar ) );
	sf::Color background_color( GetProperty<sf::Color>( "BackgroundColor", progress_bar ) );
	sf::Color progress_color( GetProperty<sf::Color>( "BarColor", progress_bar ) );
	int border_color_shift( GetProperty<int>( "BorderColorShift", progress_bar ) );
	int bar_border_color_shift( GetProperty<int>( "BarBorderColorShift", progress_bar ) );
	float border_width( GetProperty<float>( "BorderWidth", progress_bar ) );
	float bar_border_width( GetProperty<float>( "BarBorderWidth", progress_bar ) );

	ShiftBorderColors( border_color_light, border_color_dark, border_color_shift );
	ShiftBorderColors( bar_border_color_light, bar_border_color_dark, bar_border_color_shift );

	RenderQueue* queue( new RenderQueue );

	// Background.
	queue->Add(
		CreateBackground(
			sf::FloatRect(
				0.f,
				0.f,
				progress_bar->GetAllocation().Width,
				progress_bar->GetAllocation().Height
			),
			background_color
		)
	);

	if( progress_bar->GetFraction() > 0.f ) {
		sf::FloatRect bar_rect;

		if( progress_bar->GetOrientation() == ProgressBar::HORIZONTAL ) {
			float frac_width( std::max( 2.f * bar_border_width, progress_bar->GetAllocation().Width * progress_bar->GetFraction() ) );

			bar_rect = sf::FloatRect(
				std::floor( border_width + .5f ),
				std::floor( border_width + .5f ),
				std::floor( std::max( 0.f, frac_width - 2.f * border_width ) + .5f ),
				std::floor( std::max( 0.f, progress_bar->GetAllocation().Height - 2.f * border_width ) + .5f )
			);
		}
		else {
			float frac_height( std::max( 2.f * bar_border_width, progress_bar->GetAllocation().Height * progress_bar->GetFraction() ) );

			bar_rect = sf::FloatRect(
				std::floor( border_width + .5f ),
				std::floor( std::max( 0.f, progress_bar->GetAllocation().Height - frac_height + border_width ) + .5f ),
				std::floor( std::max( 0.f, progress_bar->GetAllocation().Width - 2.f * border_width ) + .5f ),
				std::floor( std::max( 0.f, frac_height - 2.f * border_width ) + .5f )
			);
		}

		// Bar.
		queue->Add(
			CreateBackground(
				bar_rect,
				progress_color
			)
		);

		// Bar border.
		queue->Add(
			CreateBorder(
				bar_rect,
				bar_border_width,
				bar_border_color_light,
				bar_border_color_dark
			)
		);
	}

	// Border.
	queue->Add(
		CreateBorder(
			sf::FloatRect(
				0.f,
				0.f,
				progress_bar->GetAllocation().Width,
				progress_bar->GetAllocation().Height
			),
			border_width,
			border_color_dark,
			border_color_light
		)
	);

	return queue;
}

}
}
