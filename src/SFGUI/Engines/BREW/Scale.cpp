#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Scale.hpp>
#include <SFGUI/RenderQueue.hpp>

namespace sfg {
namespace eng {

std::unique_ptr<RenderQueue> BREW::CreateScaleDrawable( std::shared_ptr<const Scale> scale ) const {
	auto trough_color = GetProperty<sf::Color>( "TroughColor", scale );
	auto slider_color = GetProperty<sf::Color>( "SliderColor", scale );
	auto border_color = GetProperty<sf::Color>( "BorderColor", scale );
	auto border_color_shift = GetProperty<int>( "BorderColorShift", scale );
	auto trough_thickness = GetProperty<float>( "TroughWidth", scale );
	auto border_width = GetProperty<float>( "BorderWidth", scale );

	std::unique_ptr<RenderQueue> queue( new RenderQueue );

	auto orientation = scale->GetOrientation();

	auto slider_rect = scale->GetSliderRect();

	if( orientation == Scale::Orientation::HORIZONTAL ) {
		// Trough
		queue->Add(
			Renderer::Get().CreateRect(
				sf::FloatRect(
					slider_rect.width / 2.f,
					( scale->GetAllocation().height - trough_thickness ) / 2.f,
					scale->GetAllocation().width - slider_rect.width,
					trough_thickness
				),
				trough_color
			)
		);
	}
	else {
		// Trough
		queue->Add(
			Renderer::Get().CreateRect(
				sf::FloatRect(
					( scale->GetAllocation().width - trough_thickness ) / 2.f,
					slider_rect.height / 2.f,
					trough_thickness,
					scale->GetAllocation().height - slider_rect.height
				),
				trough_color
			)
		);
	}

	// Slider
	queue->Add( CreateSlider( slider_rect, slider_color, border_width, border_color, border_color_shift ) );

	return queue;
}

}
}
