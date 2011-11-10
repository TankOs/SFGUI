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

		RenderQueue* CreateWindowDrawable( std::shared_ptr<const Window> window ) const;
		RenderQueue* CreateButtonDrawable( std::shared_ptr<const Button> button ) const;
		RenderQueue* CreateToggleButtonDrawable( std::shared_ptr<const ToggleButton> button ) const;
		RenderQueue* CreateCheckButtonDrawable( std::shared_ptr<const CheckButton> check ) const;
		RenderQueue* CreateLabelDrawable( std::shared_ptr<const Label> label ) const;
		RenderQueue* CreateEntryDrawable( std::shared_ptr<const Entry> entry ) const;
		RenderQueue* CreateScaleDrawable( std::shared_ptr<const Scale> scale ) const;
		RenderQueue* CreateScrollbarDrawable( std::shared_ptr<const Scrollbar> scrollbar ) const;
		RenderQueue* CreateScrolledWindowDrawable( std::shared_ptr<const ScrolledWindow> scrolled_window ) const;
		RenderQueue* CreateProgressBarDrawable( std::shared_ptr<const ProgressBar> progress_bar ) const;

		/** Utility function to create borders.
		 * @param rect Rectangle.
		 * @param border_width Border width.
		 * @param light_color Color of lightened edges.
		 * @param dark_color Color of darkened edges.
		 * @return RenderQueue containing border (unmanaged memory!).
		 */
		static RenderQueue* CreateBorder( const sf::FloatRect& rect, float border_width, const sf::Color& light_color, const sf::Color& dark_color );

		/** Utility function to create borders taking an absolute position.
		 * @param rect Rectangle with absolute position.
		 * @param border_width Border width.
		 * @param light_color Color of lightened edges.
		 * @param dark_color Color of darkened edges.
		 * @return RenderQueue containing border (unmanaged memory!).
		 */
		static RenderQueue* CreateAbsoluteBorder( const sf::FloatRect& rect, float border_width, const sf::Color& light_color, const sf::Color& dark_color );

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
