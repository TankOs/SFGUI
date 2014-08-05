#pragma once

#include <SFGUI/Config.hpp>

#include <SFML/System/Vector2.hpp>
#include <memory>

namespace sf {
class Image;
}

namespace sfg {

/** Primitive Texture
 */
class SFGUI_API PrimitiveTexture {
public:
	typedef std::shared_ptr<PrimitiveTexture> Ptr; //!< Shared pointer.

	~PrimitiveTexture();

	void Update( const sf::Image& data );

	sf::Vector2f offset;
	sf::Vector2u size;
};

}
