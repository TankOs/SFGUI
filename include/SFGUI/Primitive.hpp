#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/SharedPtr.hpp>
#include <SFGUI/RendererViewport.hpp>

#include <SFML/Graphics.hpp>

namespace sfg {

class SFGUI_API Primitive {
	public:
		struct Vertex {
			sf::Vector2f position;
			sf::Color color;
			sf::Vector2f texture_coordinate;
		};

		typedef SharedPtr<Primitive> Ptr;

		Primitive();

		void Add( const Primitive& primitive );

		sf::Vector2f position;
		RendererViewport::Ptr viewport;
		float layer;
		int level;

		std::vector<Vertex> vertices;

		bool synced;
		bool visible;
};

}
