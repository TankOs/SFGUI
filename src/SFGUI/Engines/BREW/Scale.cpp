#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Scale.hpp>

namespace sfg {
namespace eng {

RenderQueue* BREW::CreateScaleDrawable( SharedPtr<const Scale> scale ) const {
	sf::Color trough_color( GetProperty<sf::Color>( "TroughColor", scale ) );
	sf::Color slider_color( GetProperty<sf::Color>( "SliderColor", scale ) );
	sf::Color border_color_light( GetProperty<sf::Color>( "BorderColor", scale ) );
	sf::Color border_color_dark( GetProperty<sf::Color>( "BorderColor", scale ) );
	int border_color_shift( GetProperty<int>( "BorderColorShift", scale ) );
	float trough_thickness( GetProperty<float>( "TroughWidth", scale ) );
	float border_width( GetProperty<float>( "BorderWidth", scale ) );

	ShiftBorderColors( border_color_light, border_color_dark, border_color_shift );

	RenderQueue* queue( new RenderQueue );

	Scale::Orientation orientation = scale->GetOrientation();

	sf::FloatRect slider_rect = scale->GetSliderRect();

	if( orientation == Scale::HORIZONTAL ) {
		// Trough
		queue->Add(
			CreateBackground(
				sf::FloatRect(
					slider_rect.Width / 2.f,
					( scale->GetAllocation().Height - trough_thickness ) / 2.f,
					scale->GetAllocation().Width - slider_rect.Width,
					trough_thickness
				),
				trough_color
			)
		);
	}
	else {
		// Trough
		queue->Add(
			CreateBackground(
				sf::FloatRect(
					( scale->GetAllocation().Width - trough_thickness ) / 2.f,
					slider_rect.Height / 2.f,
					trough_thickness,
					scale->GetAllocation().Height - slider_rect.Height
				),
				trough_color
			)
		);
	}

	// Slider
	queue->Add( CreateSlider( slider_rect, slider_color, border_width, border_color_light, border_color_dark ) );

	return queue;
}

}
}
