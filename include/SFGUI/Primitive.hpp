#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/SharedPtr.hpp>

#include <SFML/Graphics.hpp>

namespace sfg {

class RendererViewport;

class SFGUI_API Primitive {
	public:
		struct Vertex {
			sf::Vector2f position;
			sf::Color color;
			sf::Vector2f texture_coordinate;
		};

		typedef SharedPtr<Primitive> Ptr;

		Primitive();

		void Add( Primitive& primitive );

		void AddVertex( const Vertex& vertex );

		void SetPosition( const sf::Vector2f& position );

		const sf::Vector2f& GetPosition() const;

		void SetViewport( const SharedPtr<RendererViewport>& viewport );

		const SharedPtr<RendererViewport>& GetViewport() const;

		void SetLayer( float layer );

		float GetLayer() const;

		void SetLevel( int level );

		int GetLevel() const;

		std::vector<Vertex>& GetVertices();

		void SetSynced( bool synced = true );

		bool IsSynced() const;

		void SetVisible( bool visible );

		bool IsVisible() const;

	private:
		sf::Vector2f m_position;
		SharedPtr<RendererViewport> m_viewport;
		float m_layer;
		int m_level;

		std::vector<Vertex> m_vertices;

		bool m_synced;
		bool m_visible;
};

}
