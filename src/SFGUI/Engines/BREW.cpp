#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/RenderQueue.hpp>
#include <SFGUI/Renderer.hpp>

#include <cmath>

namespace sfg {
namespace eng {

BREW::BREW()
{
	ResetProperties();
}

void BREW::ResetProperties() {
	// Disable automatic widget refreshing while we set all these properties.
	SetAutoRefresh( false );

	ClearProperties();

	// Set defaults.
	SetProperty( "*", "Color", sf::Color( 0xc6, 0xcb, 0xc4 ) );
	SetProperty( "*", "FontSize", 12 );
	SetProperty( "*", "FontName", "Default" ); // Use default SFGUI font when available.
	SetProperty( "*", "BackgroundColor", sf::Color( 0x46, 0x46, 0x46 ) );
	SetProperty( "*", "BorderColor", sf::Color( 0x66, 0x66, 0x66 ) );
	SetProperty( "*", "BorderColorShift", 0x20 );
	SetProperty( "*", "BorderWidth", 1.f );
	SetProperty( "*", "Padding", 5.f );
	SetProperty( "*", "Thickness", 2.f );

	// Window-specific.
	SetProperty( "Window", "Gap", 10.f );
	SetProperty( "Window", "BorderColor", sf::Color( 0x5a, 0x6a, 0x50 ) );
	SetProperty( "Window", "BorderColorShift", 0 );
	SetProperty( "Window", "TitleBackgroundColor", sf::Color( 0x5a, 0x6a, 0x50 ) );
	SetProperty( "Window", "TitlePadding", 5.f );
	SetProperty( "Window", "HandleSize", 10.f );
	SetProperty( "Window", "ShadowDistance", 3.f );
	SetProperty( "Window", "ShadowAlpha", 100.f );
	SetProperty( "Window", "CloseHeight", 10.f );
	SetProperty( "Window", "CloseThickness", 3.f );

	// Button-specific.
	SetProperty( "Button", "BackgroundColor", sf::Color( 0x55, 0x57, 0x52 ) );
	SetProperty( "Button", "BorderColor", sf::Color( 0x55, 0x57, 0x52 ) );
	SetProperty( "Button", "Spacing", 5.f );
	SetProperty( "Button:PRELIGHT", "BackgroundColor", sf::Color( 0x65, 0x67, 0x62 ) );
	SetProperty( "Button:PRELIGHT", "Color", sf::Color::White );
	SetProperty( "Button:ACTIVE", "BackgroundColor", sf::Color( 0x55, 0x55, 0x55 ) );
	SetProperty( "Button:ACTIVE", "Color", sf::Color::Black );

	// ToggleButton-specific.
	SetProperty( "ToggleButton", "BackgroundColor", sf::Color( 0x55, 0x57, 0x52 ) );
	SetProperty( "ToggleButton", "BorderColor", sf::Color( 0x55, 0x57, 0x52 ) );
	SetProperty( "ToggleButton", "Padding", 5.f );
	SetProperty( "ToggleButton:PRELIGHT", "BackgroundColor", sf::Color( 0x65, 0x67, 0x62 ) );
	SetProperty( "ToggleButton:PRELIGHT", "Color", sf::Color::White );
	SetProperty( "ToggleButton:ACTIVE", "BackgroundColor", sf::Color( 0x55, 0x55, 0x55 ) );
	SetProperty( "ToggleButton:ACTIVE", "Color", sf::Color::Black );

	// CheckButton-specific.
	SetProperty( "CheckButton", "Spacing", 5.f );
	SetProperty( "CheckButton", "BoxSize", 14.f );
	SetProperty( "CheckButton", "CheckSize", 6.f );
	SetProperty( "CheckButton", "BorderColor", sf::Color( 0x55, 0x57, 0x52 ) );
	SetProperty( "CheckButton", "BackgroundColor", sf::Color( 0x36, 0x36, 0x36 ) );
	SetProperty( "CheckButton", "CheckColor", sf::Color( 0x9e, 0x9e, 0x9e ) );
	SetProperty( "CheckButton:PRELIGHT", "BackgroundColor", sf::Color( 0x46, 0x46, 0x46 ) );
	SetProperty( "CheckButton:ACTIVE", "BackgroundColor", sf::Color( 0x56, 0x56, 0x56 ) );

	// RadioButton-specific.
	SetProperty( "RadioButton", "Spacing", 5.f );
	SetProperty( "RadioButton", "BoxSize", 14.f );
	SetProperty( "RadioButton", "CheckSize", 6.f );
	SetProperty( "RadioButton", "BorderColor", sf::Color( 0x55, 0x57, 0x52 ) );
	SetProperty( "RadioButton", "BackgroundColor", sf::Color( 0x36, 0x36, 0x36 ) );
	SetProperty( "RadioButton", "CheckColor", sf::Color( 0x9e, 0x9e, 0x9e ) );
	SetProperty( "RadioButton:PRELIGHT", "BackgroundColor", sf::Color( 0x46, 0x46, 0x46 ) );
	SetProperty( "RadioButton:ACTIVE", "BackgroundColor", sf::Color( 0x56, 0x56, 0x56 ) );

	// Entry-specific.
	SetProperty( "Entry", "BackgroundColor", sf::Color( 0x5e, 0x5e, 0x5e ) );
	SetProperty( "Entry", "Color", sf::Color::White );

	// Scale-specific.
	SetProperty( "Scale", "SliderColor", sf::Color( 0x68, 0x6a, 0x65 ) );
	SetProperty( "Scale", "SliderLength", 15.f );
	SetProperty( "Scale", "TroughColor", sf::Color( 0x70, 0x70, 0x70 ) );
	SetProperty( "Scale", "TroughWidth", 5.f );

	// Scrollbar-specific.
	SetProperty( "Scrollbar", "SliderColor", sf::Color( 0x68, 0x6a, 0x65 ) );
	SetProperty( "Scrollbar", "TroughColor", sf::Color( 0x70, 0x70, 0x70 ) );
	SetProperty( "Scrollbar", "StepperBackgroundColor", sf::Color( 0x68, 0x6a, 0x65 ) );
	SetProperty( "Scrollbar", "StepperArrowColor", sf::Color( 0xd9, 0xdc, 0xd5 ) );
	SetProperty( "Scrollbar", "StepperSpeed", 10.f );
	SetProperty( "Scrollbar", "StepperRepeatDelay", 300 );
	SetProperty( "Scrollbar", "SliderMinimumLength", 15.f );

	// ScrolledWindow-specific.
	SetProperty( "ScrolledWindow", "ScrollbarWidth", 20.f );
	SetProperty( "ScrolledWindow", "ScrollbarSpacing", 5.f );
	SetProperty( "ScrolledWindow", "BorderColor", sf::Color( 0x55, 0x57, 0x52 ) );

	// ProgressBar-specific.
	SetProperty( "ProgressBar", "BackgroundColor", sf::Color( 0x36, 0x36, 0x36 ) );
	SetProperty( "ProgressBar", "BorderColor", sf::Color( 0x55, 0x57, 0x52 ) );
	SetProperty( "ProgressBar", "BarColor", sf::Color( 0x67, 0x89, 0xab ) );
	SetProperty( "ProgressBar", "BarBorderColor", sf::Color( 0x67, 0x89, 0xab ) );
	SetProperty( "ProgressBar", "BarBorderColorShift", 0x20 );
	SetProperty( "ProgressBar", "BarBorderWidth", 1.f );

	// Separator-specific.
	SetProperty( "Separator", "Color", sf::Color( 0x75, 0x77, 0x72 ) );

	// Frame-specific.
	SetProperty( "Frame", "BorderColor", sf::Color( 0x75, 0x77, 0x72 ) );
	SetProperty( "Frame", "Padding", 7.f );
	SetProperty( "Frame", "LabelPadding", 5.f );

	// Notebook-specific.
	SetProperty( "Notebook", "BorderColor", sf::Color( 0x50, 0x52, 0x4D ) );
	SetProperty( "Notebook", "BackgroundColor", sf::Color( 0x4C, 0x4C, 0x4C ) );
	SetProperty( "Notebook", "BackgroundColorDark", sf::Color( 0x42, 0x42, 0x42 ) );
	SetProperty( "Notebook", "BackgroundColorPrelight", sf::Color( 0x48, 0x48, 0x48 ) );
	SetProperty( "Notebook", "ScrollButtonSize", 20.f );
	SetProperty( "Notebook", "ScrollButtonPrelightColor", sf::Color( 0x65, 0x67, 0x62 ) );
	SetProperty( "Notebook", "ScrollSpeed", 2.f );

	// Spinner-specific.
	SetProperty( "Spinner", "CycleDuration", 800.f );
	SetProperty( "Spinner", "Steps", 13 );
	SetProperty( "Spinner", "StoppedAlpha", 47 );
	SetProperty( "Spinner", "InnerRadius", 8.f );
	SetProperty( "Spinner", "RodThickness", 3.f );

	// ComboBox-specific.
	SetProperty( "ComboBox", "BackgroundColor", sf::Color( 0x55, 0x57, 0x52 ) );
	SetProperty( "ComboBox", "BorderColor", sf::Color( 0x55, 0x57, 0x52 ) );
	SetProperty( "ComboBox", "HighlightedColor", sf::Color( 0x65, 0x67, 0x62 ) );
	SetProperty( "ComboBox", "ArrowColor", sf::Color( 0xc6, 0xcb, 0xc4 ) );
	SetProperty( "ComboBox", "ItemPadding", 4.f );
	SetProperty( "ComboBox:PRELIGHT", "BackgroundColor", sf::Color( 0x65, 0x67, 0x62 ) );
	SetProperty( "ComboBox:ACTIVE", "BackgroundColor", sf::Color( 0x55, 0x55, 0x55 ) );

	// SpinButton-specific.
	SetProperty( "SpinButton", "BackgroundColor", sf::Color( 0x5e, 0x5e, 0x5e ) );
	SetProperty( "SpinButton", "Color", sf::Color::White );
	SetProperty( "SpinButton", "StepperAspectRatio", 1.2f );
	SetProperty( "SpinButton", "StepperBackgroundColor", sf::Color( 0x68, 0x6a, 0x65 ) );
	SetProperty( "SpinButton", "StepperArrowColor", sf::Color( 0xd9, 0xdc, 0xd5 ) );
	SetProperty( "SpinButton", "StepperSpeed", 10.f );
	SetProperty( "SpinButton", "StepperRepeatDelay", 500 );

	// (Re)Enable automatic widget refreshing after we are done setting all these properties.
	SetAutoRefresh( true );
}

std::unique_ptr<RenderQueue> BREW::CreateBorder( const sf::FloatRect& rect, float border_width, const sf::Color& light_color, const sf::Color& dark_color ) {
	border_width = std::floor( border_width + .5f );

	std::unique_ptr<RenderQueue> queue( new RenderQueue );

	// Top
	queue->Add(
		Renderer::Get().CreateLine(
			sf::Vector2f( rect.left, rect.top ),
			sf::Vector2f( rect.left + rect.width, rect.top ),
			light_color,
			border_width
		)
	);

	// Right
	queue->Add(
		Renderer::Get().CreateLine(
			sf::Vector2f( rect.left + rect.width, rect.top ),
			sf::Vector2f( rect.left + rect.width, rect.top + rect.height ),
			dark_color,
			border_width
		)
	);

	// Bottom
	queue->Add(
		Renderer::Get().CreateLine(
			sf::Vector2f( rect.left + rect.width, rect.top + rect.height ),
			sf::Vector2f( rect.left, rect.top + rect.height ),
			dark_color,
			border_width
		)
	);

	// Left
	queue->Add(
		Renderer::Get().CreateLine(
			sf::Vector2f( rect.left, rect.top + rect.height ),
			sf::Vector2f( rect.left, rect.top ),
			light_color,
			border_width
		)
	);

	return queue;
}

std::unique_ptr<RenderQueue> BREW::CreateSlider( const sf::FloatRect& rect, sf::Color& background_color, float border_width, const sf::Color& border_color, int border_color_shift ) {
	std::unique_ptr<RenderQueue> queue( new RenderQueue );

	// Align rect to pixels
	auto left = std::floor( rect.left + .5f );
	auto top = std::floor( rect.top + .5f );
	auto width = std::floor( rect.width + .5f );
	auto height = std::floor( rect.height + .5f );

	queue->Add(
		Renderer::Get().CreatePane(
			sf::Vector2f( left, top ),
			sf::Vector2f( width, height ),
			border_width,
			background_color,
			border_color,
			border_color_shift
		)
	);

	return queue;
}

std::unique_ptr<RenderQueue> BREW::CreateStepper( const sf::FloatRect& rect, sf::Color& background_color, float border_width, const sf::Color& border_color, int border_color_shift, bool pressed ) {
	std::unique_ptr<RenderQueue> queue( new RenderQueue );

	// Align rect to pixels
	auto left = std::floor( rect.left + .5f );
	auto top = std::floor( rect.top + .5f );
	auto width = std::floor( rect.width + .5f );
	auto height = std::floor( rect.height + .5f );

	if( pressed ) {
		border_color_shift = -border_color_shift;
	}

	queue->Add(
		Renderer::Get().CreatePane(
			sf::Vector2f( left, top ),
			sf::Vector2f( width, height ),
			border_width,
			background_color,
			border_color,
			border_color_shift
		)
	);

	return queue;
}

}
}
