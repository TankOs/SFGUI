#pragma once

#include <SFML/Config.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <memory>
#include <cstdint>

namespace sfg {

class Widget;

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
		TableCell( std::shared_ptr<Widget> child_, const sf::Rect<std::uint32_t>& rect_, int x_options_, int y_options_, const sf::Vector2f& padding_ );

		std::shared_ptr<Widget> child;
		sf::Rect<std::uint32_t> rect;
		int x_options;
		int y_options;
		sf::Vector2f padding;
};

}
}
