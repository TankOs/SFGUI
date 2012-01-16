#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/SharedPtr.hpp>

#include <SFML/Graphics.hpp>

namespace sfg {

class RendererViewport {
	public:
		typedef SharedPtr<RendererViewport> Ptr;

		RendererViewport();

		void Apply();

		sf::Vector2f source_origin;
		sf::Vector2f destination_origin;
		sf::Vector2f size;

		std::size_t id;

		static std::size_t last_id;
};

}
