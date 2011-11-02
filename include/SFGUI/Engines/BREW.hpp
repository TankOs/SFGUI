#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Engine.hpp>
#include <SFGUI/RenderQueue.hpp>

namespace sfg {
namespace eng {

/** BREW -- Basic Rendering Engine for Widget (reference implementation).
 */
class SFGUI_API BREW : public Engine {
	public:
		/** Ctor.
		 */
		BREW();

		sf::Drawable* CreateWindowDrawable( std::shared_ptr<Window> window ) const;
		sf::Drawable* CreateButtonDrawable( std::shared_ptr<Button> button ) const;
		sf::Drawable* CreateLabelDrawable( std::shared_ptr<Label> label ) const;
		sf::Drawable* CreateEntryDrawable( std::shared_ptr<Entry> entry ) const;
		sf::Drawable* CreateScaleDrawable( std::shared_ptr<Scale> scale ) const;
		sf::Drawable* CreateScrollbarDrawable( std::shared_ptr<Scrollbar> scrollbar ) const;
		sf::Drawable* CreateScrolledWindowDrawable( std::shared_ptr<ScrolledWindow> scrolled_window ) const;

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
