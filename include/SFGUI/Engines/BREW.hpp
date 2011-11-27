#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Engine.hpp>

namespace sfg {
class RenderQueue;

namespace eng {

/** BREW -- Basic Rendering Engine for Widget (reference implementation).
 */
class SFGUI_API BREW : public Engine {
	public:
		/** Ctor.
		 */
		BREW();

		RenderQueue* CreateWindowDrawable( SharedPtr<const Window> window ) const;
		RenderQueue* CreateButtonDrawable( SharedPtr<const Button> button ) const;
		RenderQueue* CreateToggleButtonDrawable( SharedPtr<const ToggleButton> button ) const;
		RenderQueue* CreateCheckButtonDrawable( SharedPtr<const CheckButton> check ) const;
		RenderQueue* CreateLabelDrawable( SharedPtr<const Label> label ) const;
		RenderQueue* CreateEntryDrawable( SharedPtr<const Entry> entry ) const;
		RenderQueue* CreateScaleDrawable( SharedPtr<const Scale> scale ) const;
		RenderQueue* CreateScrollbarDrawable( SharedPtr<const Scrollbar> scrollbar ) const;
		RenderQueue* CreateScrolledWindowDrawable( SharedPtr<const ScrolledWindow> scrolled_window ) const;
		RenderQueue* CreateProgressBarDrawable( SharedPtr<const ProgressBar> progress_bar ) const;
		RenderQueue* CreateSeparatorDrawable( SharedPtr<const Separator> seperator ) const;
		RenderQueue* CreateFrameDrawable( SharedPtr<const Frame> frame ) const;

		/** Utility function to create borders.
		 * @param rect Rectangle.
		 * @param border_width Border width.
		 * @param light_color Color of lightened edges.
		 * @param dark_color Color of darkened edges.
		 * @return RenderQueue containing border (unmanaged memory!).
		 */
		static RenderQueue* CreateBorder( const sf::FloatRect& rect, float border_width, const sf::Color& light_color, const sf::Color& dark_color );

		/** Utility function to create sliders.
		 * @param rect Rectangle with absolute position.
		 * @param background Background color
		 * @param border_width Border width.
		 * @param light_color Color of lightened edges.
		 * @param dark_color Color of darkened edges.
		 * @return RenderQueue containing border (unmanaged memory!).
		 */
		static RenderQueue* CreateSlider( const sf::FloatRect& rect, sf::Color& background, float border_width, const sf::Color& light_color, const sf::Color& dark_color );

		/** Utility function to create steppers.
		 * @param rect Rectangle with absolute position.
		 * @param background Background color
		 * @param border_width Border width.
		 * @param light_color Color of lightened edges.
		 * @param dark_color Color of darkened edges.
		 * @param pressed Optional parameter to indicate stepper is pressed
		 * @return RenderQueue containing border (unmanaged memory!).
		 */
		static RenderQueue* CreateStepper( const sf::FloatRect& rect, sf::Color& background, float border_width, const sf::Color& light_color, const sf::Color& dark_color, bool pressed = false );
	private:
};

}
}
