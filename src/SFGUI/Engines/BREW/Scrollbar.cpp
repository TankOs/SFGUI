#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Context.hpp>
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
		float stepper_length = scrollbar->GetAllocation().Height;

		// Trough
		queue->Add(
			Context::Get().GetRenderer().CreateRect(
				sf::FloatRect(
					stepper_length,
					0.f,
					scrollbar->GetAllocation().Width - 2.f * stepper_length,
					scrollbar->GetAllocation().Height
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
					scrollbar->GetAllocation().Height
				),
				stepper_color,
				border_width,
				stepper_border_color,
				border_color_shift,
				scrollbar->IsDecreaseStepperPressed()
			)
		);

		queue->Add(
			Context::Get().GetRenderer().CreateTriangle(
				sf::Vector2f( stepper_length * .66f, scrollbar->GetAllocation().Height * .33f ),
				sf::Vector2f( stepper_length * .33f, scrollbar->GetAllocation().Height * .5f ),
				sf::Vector2f( stepper_length * .66f, scrollbar->GetAllocation().Height * .66f ),
				stepper_arrow_color
			)
		);

		// Stepper right
		queue->Add(
			CreateStepper(
				sf::FloatRect(
					scrollbar->GetAllocation().Width - stepper_length,
					0.f,
					stepper_length,
					scrollbar->GetAllocation().Height
				),
				stepper_color,
				border_width,
				stepper_border_color,
				border_color_shift,
				scrollbar->IsIncreaseStepperPressed()
			)
		);

		queue->Add(
			Context::Get().GetRenderer().CreateTriangle(
				sf::Vector2f( scrollbar->GetAllocation().Width - stepper_length * .66f, scrollbar->GetAllocation().Height * .66f ),
				sf::Vector2f( scrollbar->GetAllocation().Width - stepper_length * .33f, scrollbar->GetAllocation().Height * .5f ),
				sf::Vector2f( scrollbar->GetAllocation().Width - stepper_length * .66f, scrollbar->GetAllocation().Height * .33f ),
				stepper_arrow_color
			)
		);
	}
	else {
		float stepper_length = scrollbar->GetAllocation().Width;

		// Trough
		queue->Add(
			Context::Get().GetRenderer().CreateRect(
				sf::FloatRect(
					0.f,
					stepper_length,
					scrollbar->GetAllocation().Width,
					scrollbar->GetAllocation().Height - 2.f * stepper_length
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
					scrollbar->GetAllocation().Width,
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
			Context::Get().GetRenderer().CreateTriangle(
				sf::Vector2f( scrollbar->GetAllocation().Width * .33f, stepper_length * .66f ),
				sf::Vector2f( scrollbar->GetAllocation().Width * .66f, stepper_length * .66f ),
				sf::Vector2f( scrollbar->GetAllocation().Width * .5f, stepper_length * .33f ),
				stepper_arrow_color
			)
		);

		// Stepper bottom
		queue->Add(
			CreateStepper(
				sf::FloatRect(
					0.f,
					scrollbar->GetAllocation().Height - stepper_length,
					scrollbar->GetAllocation().Width,
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
			Context::Get().GetRenderer().CreateTriangle(
				sf::Vector2f( scrollbar->GetAllocation().Width * .33f, scrollbar->GetAllocation().Height - stepper_length * .66f ),
				sf::Vector2f( scrollbar->GetAllocation().Width * .5f, scrollbar->GetAllocation().Height - stepper_length * .33f ),
				sf::Vector2f( scrollbar->GetAllocation().Width * .66f, scrollbar->GetAllocation().Height - stepper_length * .66f ),
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
