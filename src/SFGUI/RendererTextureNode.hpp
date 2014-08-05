#pragma once

#include <SFGUI/Config.hpp>

#include <SFML/System/Vector2.hpp>

namespace sfg {
namespace priv {

struct SFGUI_API RendererTextureNode {
	sf::Vector2i offset;
	sf::Vector2i size;
};

}
}
