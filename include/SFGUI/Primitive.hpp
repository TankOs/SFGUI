#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/PrimitiveVertex.hpp>

#include <SFML/System/Vector2.hpp>
#include <vector>
#include <memory>

namespace sfg {

class Signal;
class RendererViewport;
class PrimitiveTexture;

/** Renderer primitive.
 */
class SFGUI_API Primitive {
	public:
		typedef std::shared_ptr<Primitive> Ptr; //!< Shared pointer.

		/** Ctor.
		 * @param vertex_reserve Optional parameter hinting at how many vertices will be added to this primitive.
		 */
		Primitive( std::size_t vertex_reserve = 0 );

		/** Merge another primitive into this primitive.
		 * @param primitive Primitive to merge into this primitive.
		 */
		void Add( Primitive& primitive );

		/** Add vertex to this primitive.
		 * @param vertex Vertex to add.
		 */
		void AddVertex( const PrimitiveVertex& vertex );

		/** Add texture to this primitive.
		 * @param texture Texture to add.
		 */
		void AddTexture( std::shared_ptr<PrimitiveTexture> texture );

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
		void SetViewport( std::shared_ptr<RendererViewport> viewport );

		/** Get viewport this primitive is drawn in.
		 * @return Viewport this primitive is drawn in.
		 */
		std::shared_ptr<RendererViewport> GetViewport() const;

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
		std::vector<PrimitiveVertex>& GetVertices();

		/** Get textures in this primitive.
		 * @return Textures in this primitive.
		 */
		std::vector<std::shared_ptr<PrimitiveTexture>>& GetTextures();

		/** Get indices in this primitive.
		 * @return Indices in this primitive.
		 */
		const std::vector<unsigned int>& GetIndices() const;

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
		void SetCustomDrawCallback( std::shared_ptr<Signal> callback );

		/** Get the Signal containing the functions to call to render custom GL content.
		 * @return Signal containing the functions to call to render custom GL content.
		 */
		std::shared_ptr<Signal> GetCustomDrawCallback() const;

		/** Reset the primitive back to its default state.
		 * This clears all vertices, textures, indices and any other saved values.
		 */
		void Clear();

	private:
		sf::Vector2f m_position;
		std::shared_ptr<RendererViewport> m_viewport;
		std::shared_ptr<Signal> m_custom_draw_callback;
		int m_layer;
		int m_level;

		std::vector<PrimitiveVertex> m_vertices;
		std::vector<std::shared_ptr<PrimitiveTexture>> m_textures;
		std::vector<unsigned int> m_indices;

		bool m_synced;
		bool m_visible;
};

}
