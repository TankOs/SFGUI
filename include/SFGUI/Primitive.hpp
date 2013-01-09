#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/SharedPtr.hpp>
#include <SFGUI/Signal.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

namespace sfg {

class RendererViewport;

/** Renderer primitive.
 */
class SFGUI_API Primitive {
	public:
		typedef SharedPtr<Primitive> Ptr; //!< Shared pointer.

		/** Primitive vertex
		 */
		struct Vertex {
			sf::Vector2f position;
			sf::Color color;
			sf::Vector2f texture_coordinate;

			Vertex();

			Vertex( const Vertex& other );

			bool operator==( const Vertex& other ) const;
		};

		/** Primitive Texture
		 */
		struct Texture {
			sf::Vector2f offset;
			sf::Vector2u size;

			void Update( const sf::Image& data );

			Texture();
			~Texture();
		};

		/** Ctor.
		 * @param Optional parameter hinting at how many vertices will be added to this primitive.
		 */
		Primitive( std::size_t vertex_reserve = 0 );

		/** Merge another primitive into this primitive.
		 * @param primitive Primitive to merge into this primitive.
		 */
		void Add( Primitive& primitive );

		/** Add vertex to this primitive.
		 * @param vertex Vertex to add.
		 */
		void AddVertex( const Vertex& vertex );

		/** Add texture to this primitive.
		 * @param texture Texture to add.
		 */
		void AddTexture( const SharedPtr<Texture>& texture );

		/** Set position of this primitive.
		 * @param position Position of this primitive.
		 */
		void SetPosition( const sf::Vector2f& position );

		/** Get position of this primitive.
		 * @return Position of this primitive.
		 */
		const sf::Vector2f& GetPosition() const;

		/** Set viewport this primitive should be drawn in.
		 * @param viewport Viewport this primitive should be drawn in.
		 */
		void SetViewport( const SharedPtr<RendererViewport>& viewport );

		/** Get viewport this primitive is drawn in.
		 * @return Viewport this primitive is drawn in.
		 */
		const SharedPtr<RendererViewport>& GetViewport() const;

		/** Set draw layer of this primitive.
		 * @param layer Draw layer of this primitive.
		 */
		void SetLayer( int layer );

		/** Get draw layer of this primitive.
		 * @return Draw layer of this primitive.
		 */
		int GetLayer() const;

		/** Set draw hierarchy level of this primitive.
		 * Primitives with lower levels are drawn first.
		 * @param level New draw hierarchy level of this primitive.
		 */
		void SetLevel( int level );

		/** Get draw hierarchy level of this primitive.
		 * Primitives with lower levels are drawn first.
		 * @return Draw hierarchy level of this primitive.
		 */
		int GetLevel() const;

		/** Get vertices in this primitive.
		 * @return Vertices in this primitive.
		 */
		std::vector<Vertex>& GetVertices();

		/** Get textures in this primitive.
		 * @return Textures in this primitive.
		 */
		std::vector<SharedPtr<Texture> >& GetTextures();

		/** Get indices in this primitive.
		 * @return Indices in this primitive.
		 */
		const std::vector<GLuint>& GetIndices() const;

		/** Set whether the primitive is synced with the VBO.
		 * @param synced true to flag that primitive is synced with the VBO.
		 */
		void SetSynced( bool synced = true );

		/** Is the primitive synced with the VBO?.
		 * @return true when the primitive is synced with the VBO.
		 */
		bool IsSynced() const;

		/** Flag the primitive for drawing.
		 * @param visible true to draw the primitive.
		 */
		void SetVisible( bool visible );

		/** Is the primitive visible (drawn)?.
		 * @return true when the primitive is visible.
		 */
		bool IsVisible() const;

		/** Set the function that should be called to render custom GL content.
		 * @param callback Signal containing the functions to call.
		 */
		void SetCustomDrawCallback( const SharedPtr<Signal>& callback );

		/** Get the Signal containing the functions to call to render custom GL content.
		 * @return Signal containing the functions to call to render custom GL content.
		 */
		const SharedPtr<Signal>& GetCustomDrawCallback() const;

		/** Reset the primitive back to its default state.
		 * This clears all vertices, textures, indices and any other saved values.
		 */
		void Clear();

	private:
		sf::Vector2f m_position;
		SharedPtr<RendererViewport> m_viewport;
		SharedPtr<Signal> m_custom_draw_callback;
		int m_layer;
		int m_level;

		std::vector<Vertex> m_vertices;
		std::vector<SharedPtr<Texture> > m_textures;
		std::vector<GLuint> m_indices;

		bool m_synced;
		bool m_visible;
};

}
