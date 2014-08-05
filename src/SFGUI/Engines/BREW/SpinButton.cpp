#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/SpinButton.hpp>
#include <SFGUI/RenderQueue.hpp>

#include <SFML/Graphics/Text.hpp>

namespace sfg {
namespace eng {

std::unique_ptr<RenderQueue> BREW::CreateSpinButtonDrawable( std::shared_ptr<const SpinButton> spinbutton ) const {
	auto border_color = GetProperty<sf::Color>( "BorderColor", spinbutton );
	auto background_color = GetProperty<sf::Color>( "BackgroundColor", spinbutton );
	auto text_color = GetProperty<sf::Color>( "Color", spinbutton );
	auto cursor_color = GetProperty<sf::Color>( "Color", spinbutton );
	auto text_padding = GetProperty<float>( "Padding", spinbutton );
	auto cursor_thickness = GetProperty<float>( "Thickness", spinbutton );
	auto border_width = GetProperty<float>( "BorderWidth", spinbutton );
	auto border_color_shift = GetProperty<int>( "BorderColorShift", spinbutton );
	const auto& font_name = GetProperty<std::string>( "FontName", spinbutton );
	const auto& font = GetResourceManager().GetFont( font_name );
	auto font_size = GetProperty<unsigned int>( "FontSize", spinbutton );
	auto stepper_aspect_ratio = GetProperty<float>( "StepperAspectRatio", spinbutton );
	auto stepper_color = GetProperty<sf::Color>( "StepperBackgroundColor", spinbutton );
	auto stepper_border_color = GetProperty<sf::Color>( "BorderColor", spinbutton );
	auto stepper_arrow_color = GetProperty<sf::Color>( "StepperArrowColor", spinbutton );

	std::unique_ptr<RenderQueue> queue( new RenderQueue );

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

	auto button_width = ( spinbutton->GetAllocation().height / 2.f ) * stepper_aspect_ratio;

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

	auto line_height = GetFontLineHeight( *font, font_size );
	sf::Text vis_label( spinbutton->GetVisibleText(), *font, font_size );
	vis_label.setColor( text_color );
	vis_label.setPosition( text_padding, spinbutton->GetAllocation().height / 2.f - line_height / 2.f );

	queue->Add( Renderer::Get().CreateText( vis_label ) );

	// Draw cursor if spinbutton is active and cursor is visible.
	if( spinbutton->HasFocus() && spinbutton->IsCursorVisible() ) {
		sf::String cursor_string( spinbutton->GetVisibleText() );
		if( spinbutton->GetCursorPosition() - spinbutton->GetVisibleOffset() < static_cast<int>( cursor_string.getSize() ) ) {
			cursor_string.erase( static_cast<std::size_t>( spinbutton->GetCursorPosition() - spinbutton->GetVisibleOffset() ), cursor_string.getSize() );
		}

		// Get metrics.
		sf::Vector2f metrics( GetTextStringMetrics( cursor_string, *font, font_size ) );

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
