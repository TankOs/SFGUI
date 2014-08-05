#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Scrollbar.hpp>
#include <SFGUI/RenderQueue.hpp>

namespace sfg {
namespace eng {

std::unique_ptr<RenderQueue> BREW::CreateScrollbarDrawable( std::shared_ptr<const Scrollbar> scrollbar ) const {
	auto trough_color = GetProperty<sf::Color>( "TroughColor", scrollbar );
	auto slider_color = GetProperty<sf::Color>( "SliderColor", scrollbar );
	auto slider_border_color = GetProperty<sf::Color>( "BorderColor", scrollbar );
	auto stepper_color = GetProperty<sf::Color>( "StepperBackgroundColor", scrollbar );
	auto stepper_border_color = GetProperty<sf::Color>( "BorderColor", scrollbar );
	auto stepper_arrow_color = GetProperty<sf::Color>( "StepperArrowColor", scrollbar );
	auto border_color_shift = GetProperty<int>( "BorderColorShift", scrollbar );
	auto border_width = GetProperty<float>( "BorderWidth", scrollbar );

	std::unique_ptr<RenderQueue> queue( new RenderQueue );

	auto orientation = scrollbar->GetOrientation();

	if( orientation == Scrollbar::Orientation::HORIZONTAL ) {
		auto stepper_length = scrollbar->GetAllocation().height;

		// Trough
		queue->Add(
			Renderer::Get().CreateRect(
				sf::FloatRect(
					stepper_length,
					0.f,
					scrollbar->GetAllocation().width - 2.f * stepper_length,
					scrollbar->GetAllocation().height
				),
				trough_color
			)
		);

		// Stepper left
		queue->Add(
			CreateStepper(
				sf::FloatRect(
					0.f,
					0.f,
					stepper_length,
					scrollbar->GetAllocation().height
				),
				stepper_color,
				border_width,
				stepper_border_color,
				border_color_shift,
				scrollbar->IsDecreaseStepperPressed()
			)
		);

		queue->Add(
			Renderer::Get().CreateTriangle(
				sf::Vector2f( stepper_length * .66f, scrollbar->GetAllocation().height * .33f ),
				sf::Vector2f( stepper_length * .33f, scrollbar->GetAllocation().height * .5f ),
				sf::Vector2f( stepper_length * .66f, scrollbar->GetAllocation().height * .66f ),
				stepper_arrow_color
			)
		);

		// Stepper right
		queue->Add(
			CreateStepper(
				sf::FloatRect(
					scrollbar->GetAllocation().width - stepper_length,
					0.f,
					stepper_length,
					scrollbar->GetAllocation().height
				),
				stepper_color,
				border_width,
				stepper_border_color,
				border_color_shift,
				scrollbar->IsIncreaseStepperPressed()
			)
		);

		queue->Add(
			Renderer::Get().CreateTriangle(
				sf::Vector2f( scrollbar->GetAllocation().width - stepper_length * .66f, scrollbar->GetAllocation().height * .66f ),
				sf::Vector2f( scrollbar->GetAllocation().width - stepper_length * .33f, scrollbar->GetAllocation().height * .5f ),
				sf::Vector2f( scrollbar->GetAllocation().width - stepper_length * .66f, scrollbar->GetAllocation().height * .33f ),
				stepper_arrow_color
			)
		);
	}
	else {
		auto stepper_length = scrollbar->GetAllocation().width;

		// Trough
		queue->Add(
			Renderer::Get().CreateRect(
				sf::FloatRect(
					0.f,
					stepper_length,
					scrollbar->GetAllocation().width,
					scrollbar->GetAllocation().height - 2.f * stepper_length
				),
				trough_color
			)
		);

		// Stepper top
		queue->Add(
			CreateStepper(
				sf::FloatRect(
					0.f,
					0.f,
					scrollbar->GetAllocation().width,
					stepper_length
				),
				stepper_color,
				border_width,
				stepper_border_color,
				border_color_shift,
				scrollbar->IsDecreaseStepperPressed()
			)
		);

		queue->Add(
			Renderer::Get().CreateTriangle(
				sf::Vector2f( scrollbar->GetAllocation().width * .33f, stepper_length * .66f ),
				sf::Vector2f( scrollbar->GetAllocation().width * .66f, stepper_length * .66f ),
				sf::Vector2f( scrollbar->GetAllocation().width * .5f, stepper_length * .33f ),
				stepper_arrow_color
			)
		);

		// Stepper bottom
		queue->Add(
			CreateStepper(
				sf::FloatRect(
					0.f,
					scrollbar->GetAllocation().height - stepper_length,
					scrollbar->GetAllocation().width,
					stepper_length
				),
				stepper_color,
				border_width,
				stepper_border_color,
				border_color_shift,
				scrollbar->IsIncreaseStepperPressed()
			)
		);

		queue->Add(
			Renderer::Get().CreateTriangle(
				sf::Vector2f( scrollbar->GetAllocation().width * .33f, scrollbar->GetAllocation().height - stepper_length * .66f ),
				sf::Vector2f( scrollbar->GetAllocation().width * .5f, scrollbar->GetAllocation().height - stepper_length * .33f ),
				sf::Vector2f( scrollbar->GetAllocation().width * .66f, scrollbar->GetAllocation().height - stepper_length * .66f ),
				stepper_arrow_color
			)
		);
	}

	// Slider
	queue->Add( CreateSlider( scrollbar->GetSliderRect(), slider_color, border_width, slider_border_color, border_color_shift ) );

	return queue;
}

}
}
