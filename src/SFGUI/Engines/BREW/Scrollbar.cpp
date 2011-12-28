#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Scrollbar.hpp>

#include <SFML/Graphics/ConvexShape.hpp>

namespace sfg {
namespace eng {

RenderQueue* BREW::CreateScrollbarDrawable( SharedPtr<const Scrollbar> scrollbar ) const {
	sf::Color trough_color( GetProperty<sf::Color>( "TroughColor", scrollbar ) );
	sf::Color slider_color( GetProperty<sf::Color>( "SliderColor", scrollbar ) );
	sf::Color slider_border_color_light( GetProperty<sf::Color>( "BorderColor", scrollbar ) );
	sf::Color slider_border_color_dark( GetProperty<sf::Color>( "BorderColor", scrollbar ) );
	sf::Color stepper_color( GetProperty<sf::Color>( "StepperBackgroundColor", scrollbar ) );
	sf::Color stepper_border_color_light( GetProperty<sf::Color>( "BorderColor", scrollbar ) );
	sf::Color stepper_border_color_dark( GetProperty<sf::Color>( "BorderColor", scrollbar ) );
	sf::Color stepper_arrow_color( GetProperty<sf::Color>( "StepperArrowColor", scrollbar ) );
	int border_color_shift( GetProperty<int>( "BorderColorShift", scrollbar ) );
	float border_width( GetProperty<float>( "BorderWidth", scrollbar ) );

	ShiftBorderColors( slider_border_color_light, slider_border_color_dark, border_color_shift );
	ShiftBorderColors( stepper_border_color_light, stepper_border_color_dark, border_color_shift );

	RenderQueue* queue( new RenderQueue );

	Scrollbar::Orientation orientation = scrollbar->GetOrientation();

	if( orientation == Scrollbar::HORIZONTAL ) {
		float stepper_length = scrollbar->GetAllocation().Height;

		// Trough
		queue->Add(
			CreateBackground(
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
				stepper_border_color_light,
				stepper_border_color_dark,
				scrollbar->IsDecreaseStepperPressed()
			)
		);

		sf::ConvexShape* arrow_left = new sf::ConvexShape( 3 );
		arrow_left->SetPoint(
			0,
			sf::Vector2f(
				stepper_length * .66f,
				scrollbar->GetAllocation().Height * .33f
			)
		);
		arrow_left->SetPoint(
			1,
			sf::Vector2f(
				stepper_length * .33f,
				scrollbar->GetAllocation().Height * .5f
			)
		);
		arrow_left->SetPoint(
			2,
			sf::Vector2f(
				stepper_length * .66f,
				scrollbar->GetAllocation().Height * .66f
			)
		);

		queue->Add(arrow_left);

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
				stepper_border_color_light,
				stepper_border_color_dark,
				scrollbar->IsIncreaseStepperPressed()
			)
		);

		sf::ConvexShape* arrow_right = new sf::ConvexShape( 3 );
		arrow_right->SetPoint(
			0,
			sf::Vector2f(
				scrollbar->GetAllocation().Width - stepper_length * .66f,
				scrollbar->GetAllocation().Height * .33f
			)
		);
		arrow_right->SetPoint(
			1,
			sf::Vector2f(
				scrollbar->GetAllocation().Width - stepper_length * .33f,
				scrollbar->GetAllocation().Height * .5f
			)
		);
		arrow_right->SetPoint(
			2,
			sf::Vector2f(
				scrollbar->GetAllocation().Width - stepper_length * .66f,
				scrollbar->GetAllocation().Height * .66f
			)
		);

		queue->Add(arrow_right);
	}
	else {
		float stepper_length = scrollbar->GetAllocation().Width;

		// Trough
		queue->Add(
			CreateBackground(
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
				stepper_border_color_light,
				stepper_border_color_dark,
				scrollbar->IsDecreaseStepperPressed()
			)
		);

		sf::ConvexShape* arrow_up = new sf::ConvexShape( 3 );
		arrow_up->SetPoint(
			0,
			sf::Vector2f(
				scrollbar->GetAllocation().Width * .33f,
				stepper_length * .66f
			)
		);
		arrow_up->SetPoint(
			1,
			sf::Vector2f(
				scrollbar->GetAllocation().Width * .66f,
				stepper_length * .66f
			)
		);
		arrow_up->SetPoint(
			2,
			sf::Vector2f(
				scrollbar->GetAllocation().Width * .5f,
				stepper_length * .33f
			)
		);

		queue->Add(arrow_up);

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
				stepper_border_color_light,
				stepper_border_color_dark,
				scrollbar->IsIncreaseStepperPressed()
			)
		);

		sf::ConvexShape* arrow_down = new sf::ConvexShape( 3 );
		arrow_down->SetOutlineColor( sf::Color::Transparent );
		arrow_down->SetFillColor( stepper_arrow_color );
		arrow_down->SetPoint(
			0,
			sf::Vector2f(
				scrollbar->GetAllocation().Width * .33f,
				scrollbar->GetAllocation().Height - stepper_length * .66f
			)
		);
		arrow_down->SetPoint(
			1,
			sf::Vector2f(
				scrollbar->GetAllocation().Width * .66f,
				scrollbar->GetAllocation().Height - stepper_length * .66f
			)
		);
		arrow_down->SetPoint(
			2,
			sf::Vector2f(
				scrollbar->GetAllocation().Width * .5f,
				scrollbar->GetAllocation().Height - stepper_length * .33f
			)
		);

		queue->Add(arrow_down);
	}

	// Slider
	queue->Add( CreateSlider( scrollbar->GetSliderRect(), slider_color, border_width, slider_border_color_light, slider_border_color_dark ) );

	return queue;
}

}
}
