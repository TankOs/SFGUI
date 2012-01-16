#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/SharedPtr.hpp>
#include <SFGUI/Primitive.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <vector>
#include <map>

namespace sfg {

class SFGUI_API Renderer {
	public:
		Renderer();

		~Renderer();

		const RendererViewport::Ptr& GetDefaultViewport();

		RendererViewport::Ptr CreateViewport();

		Primitive::Ptr CreateText( const sf::Text& text, sf::Color background_color_hint );

		Primitive::Ptr CreateQuad( const sf::Vector2f& top_left, const sf::Vector2f& bottom_left,
		                           const sf::Vector2f& bottom_right, const sf::Vector2f& top_right,
		                           const sf::Color& color = sf::Color::White );

		Primitive::Ptr CreatePane( const sf::Vector2f& position, const sf::Vector2f& size, float border_width,
		                           const sf::Color& color = sf::Color::White, const sf::Color& border_color = sf::Color::Black,
		                           int border_color_shift = 0 );

		Primitive::Ptr CreateRect( const sf::Vector2f& top_left, const sf::Vector2f& bottom_right, const sf::Color& color = sf::Color::White );

		Primitive::Ptr CreateRect( const sf::FloatRect& rect, const sf::Color& color = sf::Color::White );

		Primitive::Ptr CreateTriangle( const sf::Vector2f& point0, const sf::Vector2f& point1, const sf::Vector2f& point2, const sf::Color& color = sf::Color::White );

		Primitive::Ptr CreateImage( const sf::FloatRect& rect, const sf::Image& image, sf::Color background_color_hint );

		Primitive::Ptr CreateLine( const sf::Vector2f& begin, const sf::Vector2f& end, const sf::Color& color = sf::Color::White, float thickness = 1.f );

		void RemovePrimitive( const Primitive::Ptr& primitive );

		void InvalidateVBO();

		void Draw( const Primitive::Ptr& primitive );

		void Display( sf::RenderWindow& window );

		void TuneDepthTest( bool enable );

		void TuneAlphaThreshold( float alpha_threshold );

		void TunePrecomputeBlending( bool enable );

	private:
		void SetupGL( sf::RenderWindow& window );

		void RestoreGL( sf::RenderWindow& window );

		void UpdateVBO();

		void SortPrimitives();

		void RemoveExpired();

		void RefreshVBO();

		sf::Vector2f LoadFont( const sf::Font& font, unsigned int size, sf::Color background_color_hint, sf::Color foreground_color_hint );

		sf::Vector2f LoadImage( const sf::Image& image, sf::Color background_color_hint, sf::Color foreground_color_hint = sf::Color( 0, 0, 0, 0 ) );

		typedef std::pair<RendererViewport::Ptr, unsigned int> ViewportPair;

		std::vector<Primitive::Ptr> m_primitives;
		std::vector<RendererViewport::Ptr> m_viewports;

		std::vector<sf::Vector3f> m_vertex_data;
		std::vector<sf::Color> m_color_data;
		std::vector<sf::Vector2f> m_texture_data;

		std::vector<ViewportPair> m_viewport_pairs;

		RendererViewport::Ptr m_default_viewport;

		sf::Texture m_texture_atlas;

		std::map<unsigned long, sf::Vector2f> m_atlas_offsets;

		GLuint m_vertex_vbo;
		GLuint m_color_vbo;
		GLuint m_texture_vbo;

		GLsizei m_last_vertex_count;

		float m_alpha_threshold;

		bool m_vbo_synced;
		bool m_depth_test;
		bool m_preblend;
};

}
