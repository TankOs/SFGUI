#pragma once

#include <SFGUI/Widget.hpp>
#include <SFGUI/Container.hpp>

#include <SFML/Graphics/Rect.hpp>

namespace sfg {
namespace priv {

/** Table cell.
 * For internal use only.
 */
class TableCell {
	public:
		/** Ctor.
		 * @param child_ Child widget.
		 * @param rect_ Rect.
		 * @param x_options_ X options.
		 * @param y_options_ Y options.
		 * @param padding_ Padding.
		 */
		TableCell( const Widget::Ptr& child_, const sf::Rect<sf::Uint32>& rect_, int x_options_, int y_options_, const sf::Vector2f& padding_ );

		sfg::Widget::Ptr child;
		sf::Rect<sf::Uint32> rect;
		int x_options;
		int y_options;
		sf::Vector2f padding;
};

}
}
