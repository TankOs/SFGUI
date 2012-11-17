#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/RenderQueue.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Window.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Entry.hpp>
#include <SFGUI/Scale.hpp>
#include <SFGUI/Scrollbar.hpp>
#include <SFGUI/ScrolledWindow.hpp>
#include <SFGUI/ToggleButton.hpp>
#include <SFGUI/CheckButton.hpp>
#include <SFGUI/ProgressBar.hpp>
#include <SFGUI/Separator.hpp>
#include <SFGUI/Frame.hpp>
#include <SFGUI/Image.hpp>
#include <SFGUI/Notebook.hpp>
#include <SFGUI/Spinner.hpp>
#include <SFGUI/ComboBox.hpp>

#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <cmath>

namespace sfg {
namespace eng {

Bob::Bob() :
	Engine()
{
	ResetProperties();
}

void Bob::ResetProperties() {
	ClearProperties();

	// Set defaults.
	SetProperty( "*", "Color", sf::Color::Black );
	SetProperty( "*", "FontSize", 14 );
	SetProperty( "*", "FontName", "" ); // Uses SFGUI's default font when empty.
	SetProperty( "*", "BackgroundColor", sf::Color::White );
	SetProperty( "*", "Padding", 5.f );

	// Button-specific
	SetProperty( "Button", "Spacing", 5.f );

	// Entry-specific
	SetProperty( "Entry", "Thickness", 1.f );

	// Scale-specific
	SetProperty( "Scale", "SliderLength", 15.f );
	SetProperty( "Scale", "TroughWidth", 5.f );

	// Scrollbar-specific.
	SetProperty( "Scrollbar", "StepperSpeed", 10.f );
	SetProperty( "Scrollbar", "StepperRepeatDelay", 300 );
	SetProperty( "Scrollbar", "SliderMinimumLength", 15.f );

	// ScrolledWindow-specific
	SetProperty( "ScrolledWindow", "ScrollbarWidth", 20.f );
	SetProperty( "ScrolledWindow", "ScrollbarSpacing", 5.f );
	SetProperty( "ScrolledWindow", "BorderWidth", 0.f );

	// Window-specific
	SetProperty( "Window", "Gap", 10.f );
	SetProperty( "Window", "TitlePadding", 5.f );
	SetProperty( "Window", "HandleSize", 10.f );
}

}
}
