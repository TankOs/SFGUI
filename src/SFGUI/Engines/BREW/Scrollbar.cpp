#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Scrollbar.hpp>

namespace sfg {
namespace eng {

RenderQueue* BREW::CreateScrollbarDrawable( SharedPtr<const Scrollbar> scrollbar ) const {
	sf::Color trough_color( GetProperty<sf::Color>( "TroughColor", scrollbar ) );
	sf::Color slider_color( GetProperty<sf::Color>( "SliderColor", scrollbar ) );
	sf::Color slider_border_color( GetProperty<sf::Color>( "BorderColor", scrollbar ) );
	sf::Color stepper_color( GetProperty<sf::Color>( "StepperBackgroundColor", scrollbar ) );
	sf::Color stepper_border_color( GetProperty<sf::Color>( "BorderColor", scrollbar ) );
	sf::Color stepper_arrow_color( GetProperty<sf::Color>( "StepperArrowColor", scrollbar ) );
	int border_color_shift( GetProperty<int>( "BorderColorShift", scrollbar ) );
	float border_width( GetProperty<float>( "BorderWidth", scrollbar ) );

	RenderQueue* queue( new RenderQueue );

	Scrollbar::Orientation orientation = scrollbar->GetOrientation();

	if( orientation == Scrollbar::HORIZONTAL ) {
		float stepper_length = scrollbar->GetAllocation().height;

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
		float stepper_length = scrollbar->GetAllocation().width;

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
