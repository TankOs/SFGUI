#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/SpinButton.hpp>

#include <SFML/Graphics/Text.hpp>

namespace sfg {
namespace eng {

RenderQueue* BREW::CreateSpinButtonDrawable( SharedPtr<const SpinButton> spinbutton ) const {
	sf::Color border_color( GetProperty<sf::Color>( "BorderColor", spinbutton ) );
	sf::Color background_color( GetProperty<sf::Color>( "BackgroundColor", spinbutton ) );
	sf::Color text_color( GetProperty<sf::Color>( "Color", spinbutton ) );
	sf::Color cursor_color( GetProperty<sf::Color>( "Color", spinbutton ) );
	float text_padding( GetProperty<float>( "Padding", spinbutton ) );
	float cursor_thickness( GetProperty<float>( "Thickness", spinbutton ) );
	float border_width( GetProperty<float>( "BorderWidth", spinbutton ) );
	int border_color_shift( GetProperty<int>( "BorderColorShift", spinbutton ) );
	const sf::Font& font( *GetResourceManager().GetFont( GetProperty<std::string>( "FontName", spinbutton ) ) );
	const unsigned int& font_size( GetProperty<unsigned int>( "FontSize", spinbutton ) );
	float stepper_aspect_ratio( GetProperty<float>( "StepperAspectRatio", spinbutton ) );
	sf::Color stepper_color( GetProperty<sf::Color>( "StepperBackgroundColor", spinbutton ) );
	sf::Color stepper_border_color( GetProperty<sf::Color>( "BorderColor", spinbutton ) );
	sf::Color stepper_arrow_color( GetProperty<sf::Color>( "StepperArrowColor", spinbutton ) );

	RenderQueue* queue( new RenderQueue );

	// Pane.
	queue->Add(
		Renderer::Get().CreatePane(
			sf::Vector2f( 0.f, 0.f ),
			sf::Vector2f( spinbutton->GetAllocation().width, spinbutton->GetAllocation().height ),
			border_width,
			background_color,
			border_color,
			-border_color_shift
		)
	);

	float button_width = ( spinbutton->GetAllocation().height / 2.f ) * stepper_aspect_ratio;

	// Up Stepper.
	queue->Add(
		Renderer::Get().CreatePane(
			sf::Vector2f( spinbutton->GetAllocation().width - button_width - border_width, border_width ),
			sf::Vector2f( button_width, spinbutton->GetAllocation().height / 2.f - border_width ),
			border_width,
			stepper_color,
			stepper_border_color,
			spinbutton->IsIncreaseStepperPressed() ? -border_color_shift : border_color_shift
		)
	);

	// Up Stepper Triangle.
	queue->Add(
		Renderer::Get().CreateTriangle(
			sf::Vector2f( spinbutton->GetAllocation().width - button_width / 2.f - border_width, ( spinbutton->IsIncreaseStepperPressed() ? 1.f : 0.f ) + border_width + spinbutton->GetAllocation().height / 6.f ),
			sf::Vector2f( spinbutton->GetAllocation().width - button_width / 4.f * 3.f - border_width, ( spinbutton->IsIncreaseStepperPressed() ? 1.f : 0.f ) + border_width + spinbutton->GetAllocation().height / 3.f ),
			sf::Vector2f( spinbutton->GetAllocation().width - button_width / 4.f - border_width, ( spinbutton->IsIncreaseStepperPressed() ? 1.f : 0.f ) + border_width + spinbutton->GetAllocation().height / 3.f ),
			stepper_arrow_color
		)
	);

	// Down Stepper.
	queue->Add(
		Renderer::Get().CreatePane(
			sf::Vector2f( spinbutton->GetAllocation().width - button_width - border_width, spinbutton->GetAllocation().height / 2.f ),
			sf::Vector2f( button_width, spinbutton->GetAllocation().height / 2.f - border_width ),
			border_width,
			stepper_color,
			stepper_border_color,
			spinbutton->IsDecreaseStepperPressed() ? -border_color_shift : border_color_shift
		)
	);

	// Down Stepper Triangle.
	queue->Add(
		Renderer::Get().CreateTriangle(
			sf::Vector2f( spinbutton->GetAllocation().width - button_width / 2.f - border_width, ( spinbutton->IsDecreaseStepperPressed() ? 1.f : 0.f ) + spinbutton->GetAllocation().height - border_width - spinbutton->GetAllocation().height / 6.f ),
			sf::Vector2f( spinbutton->GetAllocation().width - button_width / 4.f - border_width, ( spinbutton->IsDecreaseStepperPressed() ? 1.f : 0.f ) + spinbutton->GetAllocation().height - border_width - spinbutton->GetAllocation().height / 3.f ),
			sf::Vector2f( spinbutton->GetAllocation().width - button_width / 4.f * 3.f - border_width, ( spinbutton->IsDecreaseStepperPressed() ? 1.f : 0.f ) + spinbutton->GetAllocation().height - border_width - spinbutton->GetAllocation().height / 3.f ),
			stepper_arrow_color
		)
	);

	float line_height = GetFontLineHeight( font, font_size );
	sf::Text vis_label( spinbutton->GetVisibleText(), font, font_size );
	vis_label.setColor( text_color );
	vis_label.setPosition( text_padding, spinbutton->GetAllocation().height / 2.f - line_height / 2.f );

	queue->Add( Renderer::Get().CreateText( vis_label ) );

	// Draw cursor if spinbutton is active and cursor is visible.
	if( spinbutton->HasFocus() && spinbutton->IsCursorVisible() ) {
		sf::String cursor_string( spinbutton->GetVisibleText() );
		if( spinbutton->GetCursorPosition() - spinbutton->GetVisibleOffset() < cursor_string.getSize() ) {
			cursor_string.erase( spinbutton->GetCursorPosition() - spinbutton->GetVisibleOffset(), cursor_string.getSize() );
		}

		// Get metrics.
		sf::Vector2f metrics( GetTextMetrics( cursor_string, font, font_size ) );

		queue->Add(
			Renderer::Get().CreateRect(
				sf::FloatRect(
					metrics.x + text_padding,
					spinbutton->GetAllocation().height / 2.f - line_height / 2.f,
					cursor_thickness,
					line_height
				),
				cursor_color
			)
		);
	}

	return queue;
}

}
}
