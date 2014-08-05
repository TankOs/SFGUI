#pragma once

#include <SFGUI/Config.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>

namespace sfg {

/** Primitive vertex
 */
class SFGUI_API PrimitiveVertex {
public:
	PrimitiveVertex();

	PrimitiveVertex( const PrimitiveVertex& other );

	bool operator==( const PrimitiveVertex& other ) const;

	sf::Vector2f position;
	sf::Color color;
	sf::Vector2f texture_coordinate;
};

}
