#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/SharedPtr.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <vector>
#include <map>

namespace sfg {

class SFGUI_API ProjectO {
	public:
		struct Primitive {
			struct Vertex {
				sf::Vector2f position;
				sf::Color color;
				sf::Vector2f texture_coordinate;
			};

			sf::Vector2f position;
			float layer;
			int level;

			std::vector<Vertex> vertices;

			bool synced;
			bool visible;
		};

		typedef WeakPtr<Primitive> PrimitiveWeakPtr;
		typedef SharedPtr<Primitive> PrimitivePtr;

		ProjectO();

		~ProjectO();

		PrimitivePtr CreateText( const sf::Text& text );

		PrimitivePtr CreateQuad( const sf::Vector2f& top_left, const sf::Vector2f& bottom_left,
		                          const sf::Vector2f& bottom_right, const sf::Vector2f& top_right,
		                          const sf::Color& color = sf::Color::White );

		PrimitivePtr CreateRect( const sf::Vector2f& top_left, const sf::Vector2f& bottom_right, const sf::Color& color = sf::Color::White );

		PrimitivePtr CreateRect( const sf::FloatRect& rect, const sf::Color& color = sf::Color::White );

		PrimitivePtr CreateTriangle( const sf::Vector2f& point0, const sf::Vector2f& point1, const sf::Vector2f& point2, const sf::Color& color = sf::Color::White );

		PrimitivePtr CreateImage( const sf::FloatRect& rect, const sf::Image& image );

		PrimitivePtr CreateLine( const sf::Vector2f& begin, const sf::Vector2f& end, const sf::Color& color = sf::Color::White, float thickness = 1.f );

		void Draw( const PrimitivePtr& primitive );

		void Display( sf::RenderWindow& window );

	private:
		void SetupGL( sf::RenderWindow& window );

		void RestoreGL( sf::RenderWindow& window );

		void UpdateVBO();

		sf::Vector2f LoadFont( const sf::Font& font, unsigned int size );

		sf::Vector2f LoadImage( const sf::Image& image );

		std::vector<PrimitivePtr> m_primitives;

		std::vector<sf::Vector2f> m_vertex_data;
		std::vector<sf::Color> m_color_data;
		std::vector<sf::Vector2f> m_texture_data;

		sf::Texture m_texture_atlas;

		std::map<unsigned long, sf::Vector2f> m_atlas_offsets;

		GLuint m_vertex_vbo;
		GLuint m_color_vbo;
		GLuint m_texture_vbo;

		GLsizei m_last_vertex_count;

		bool m_vbo_synced;
};

}
