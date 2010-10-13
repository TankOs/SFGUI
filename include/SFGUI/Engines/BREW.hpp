#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/RenderEngine.hpp>
#include <SFGUI/RenderQueue.hpp>

namespace sfg {
namespace eng {

/** BREW -- Basic Rendering Engine for Widget (reference implementation).
 */
class BREW : public RenderEngine {
	public:
		sf::Drawable* CreateWindowDrawable( Window::Ptr window ) const;
		sf::Drawable* CreateButtonDrawable( Button::Ptr button ) const;

		/** Utility function to create borders.
		 * @param rect Rectangle.
		 * @param border_width Border width.
		 * @param light_color Color of lightened edges.
		 * @param dark_color Color of darkened edges.
		 * @return RenderQueue containing border (unmanaged memory!).
		 */
		static RenderQueue* CreateBorder( const sf::FloatRect& rect, float border_width, const sf::Color& light_color, const sf::Color& dark_color );
};

}
}
