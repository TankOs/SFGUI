#pragma once

#include <SFGUI/Config.hpp>
#include <SFML/Graphics/Color.hpp>
#include <string>

namespace sfg {

/** Theme loader to feed RenderEngine objects.
 */
class SFGUI_API Theme {
	public:
		static sf::Color ParseColor( const std::string& str, const sf::Color& default_ = sf::Color( 255, 0, 0 ) );

	private:
};

}
