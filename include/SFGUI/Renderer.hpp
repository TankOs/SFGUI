#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/SharedPtr.hpp>
#include <SFGUI/Primitive.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <vector>
#include <map>

namespace sfg {

class RendererViewport;

/** SFGUI VBO renderer.
 */
class SFGUI_API Renderer {
	public:
		enum DepthClearStrategy {
			NO_DEPTH = 0, //!< No depth testing.
			CLEAR_DEPTH = 1 << 0, //!< Depth testing using glClear().
			ALTERNATE_DEPTH = 1 << 1, //!< Depth testing alternating between GL_LESS and GL_GREATER.
			DEFAULT = NO_DEPTH //!< Default: No depth testing.
		};

		enum InvalidateDataset {
			INVALIDATE_VERTEX = 1 << 0, //!< Vertex data needs a sync.
			INVALIDATE_COLOR = 1 << 1, //!< Color data needs a sync.
			INVALIDATE_TEXTURE = 1 << 2, //!< Texture data needs a sync.
			INVALIDATE_INDEX = 1 << 3, //!< Index data needs a sync.
			INVALIDATE_ALL = INVALIDATE_VERTEX | INVALIDATE_COLOR | INVALIDATE_TEXTURE | INVALIDATE_INDEX //!< All data needs a sync.
		};

		/** Create the Renderer singleton instance.
		 * @return Renderer instance.
		 */
		static Renderer& Create();

		/** Get the Renderer singleton instance.
		 * @return Renderer instance.
		 */
		static Renderer& Get();

		/** Destroy the Renderer singleton instance.
		 */
		static void Destroy();

		/** Dtor.
		 */
		~Renderer();

		/** Get default viewport that covers the entire window.
		 * @return Default viewport that covers the entire window.
		 */
		const SharedPtr<RendererViewport>& GetDefaultViewport();

		/** Create and register a new viewport with the renderer.
		 * @return New viewport.
		 */
		SharedPtr<RendererViewport> CreateViewport();

		/** Create and register a new text primitive with the renderer.
		 * @param text sf::Text describing the text to be drawn.
		 * @param background_color_hint Background color hint for pre-blending.
		 * @return New text primitive.
		 */
		Primitive::Ptr CreateText( const sf::Text& text );

		/** Create and register a new quad primitive with the renderer.
		 * @param top_left Top left corner of the quad.
		 * @param bottom_left Bottom left corner of the quad.
		 * @param bottom_right Bottom right corner of the quad.
		 * @param top_right Top right corner of the quad.
		 * @param color Color of the quad.
		 * @return New quad primitive.
		 */
		Primitive::Ptr CreateQuad( const sf::Vector2f& top_left, const sf::Vector2f& bottom_left,
		                           const sf::Vector2f& bottom_right, const sf::Vector2f& top_right,
		                           const sf::Color& color = sf::Color::White );

		/** Create and register a new pane (rect with border) primitive with the renderer.
		 * @param position Position of the pane (top left corner).
		 * @param size Size of the pane.
		 * @param border_width Width of the border.
		 * @param color Color of the rect.
		 * @param border_color Color of the border.
		 * @param border_color_shift Amount to shift border colors by to create a 3D effect.
		 * @return New pane primitive.
		 */
		Primitive::Ptr CreatePane( const sf::Vector2f& position, const sf::Vector2f& size, float border_width,
		                           const sf::Color& color = sf::Color::White, const sf::Color& border_color = sf::Color::Black,
		                           int border_color_shift = 0 );

		/** Create and register a new rect primitive with the renderer.
		 * @param top_left Top left corner of the rect.
		 * @param bottom_right Bottom right corner of the rect.
		 * @param color Color of the rect.
		 * @return New rect primitive.
		 */
		Primitive::Ptr CreateRect( const sf::Vector2f& top_left, const sf::Vector2f& bottom_right, const sf::Color& color = sf::Color::White );

		/** Create and register a new rect primitive with the renderer.
		 * @param rect sf::FloatRect describing the rect to be constructed.
		 * @param color Color of the rect.
		 * @return New rect primitive.
		 */
		Primitive::Ptr CreateRect( const sf::FloatRect& rect, const sf::Color& color = sf::Color::White );

		/** Create and register a new triangle primitive with the renderer.
		 * Keep in mind that the points have to be specified in counter-clockwise order.
		 * @param point0 First point of the triangle.
		 * @param point1 Second point of the triangle.
		 * @param point2 Third point of the triangle.
		 * @param color Color of the triangle.
		 * @return New triangle primitive.
		 */
		Primitive::Ptr CreateTriangle( const sf::Vector2f& point0, const sf::Vector2f& point1, const sf::Vector2f& point2, const sf::Color& color = sf::Color::White );

		/** Create and register a new image primitive with the renderer.
		 * @param rect sf::FloatRect describing the rect in which the image should be drawn.
		 * @param image sf::Image containing the image to draw.
		 * @param background_color_hint Background color hint for pre-blending.
		 * @return New image primitive.
		 */
		Primitive::Ptr CreateImage( const sf::FloatRect& rect, const sf::Image& image );

		/** Create and register a new line primitive with the renderer.
		 * @param begin Starting point of the line.
		 * @param end End point of the line.
		 * @param color Color of the line.
		 * @param thickness Thickness of the line.
		 * @return New line primitive.
		 */
		Primitive::Ptr CreateLine( const sf::Vector2f& begin, const sf::Vector2f& end, const sf::Color& color = sf::Color::White, float thickness = 1.f );

		/** Register a primitive with the renderer.
		 * @param primitive Primitive to be registered.
		 */
		void AddPrimitive( const Primitive::Ptr& primitive );

		/** Unregister a primitive from the renderer.
		 * @param primitive Primitive to be unregistered.
		 */
		void RemovePrimitive( const Primitive::Ptr& primitive );

		/** Invalidate VBO data so it is resynchronized with fresh data.
		 * @param datasets The datasets to invalidate. Default: INVALIDATE_ALL
		 * Bitwise OR of INVALIDATE_VERTEX, INVALIDATE_COLOR, INVALIDATE_TEXTURE or INVALIDATE_INDEX.
		 */
		void InvalidateVBO( unsigned char datasets = INVALIDATE_ALL );

		/** Draw the GUI.
		 * @param target sf::RenderTarget to draw to.
		 */
		void Display( sf::RenderTarget& target ) const;

		/** Enable and select depth testing method.
		 * WARNING: THIS FEATURE IS BROKEN AND THEREFORE DISABLED UNTIL FURTHER NOTICE.
		 * Renderer::NO_DEPTH To disable depth testing.
		 * Renderer::CLEAR_DEPTH To enable depth testing and running glClear() every frame.
		 * Renderer::ALTERNATE_DEPTH To enable depth testing and alternate between GL_LESS and GL_GREATER instead of clearing the depth buffer every frame. Use this only if you don't use the depth buffer yourself.
		 * @param strategy Depth buffer strategy to use (default: NO_DEPTH).
		 */
		void TuneDepthTest( unsigned char strategy );

		/** Enable and select alpha testing threshold.
		 * @param alpha_threshold Threshold at which fragments will get discarded if their alpha value is less than or equal to. Set to 0.f to disable.
		 */
		void TuneAlphaThreshold( float alpha_threshold );

		/** Enable or disable CPU driven face culling.
		 * @param enable true to enable, false to disable.
		 */
		void TuneCull( bool enable );

		/** Enable or disable FBO GUI caching.
		 * @param enable true to enable, false to disable.
		 */
		void TuneUseFBO( bool enable );

	private:
		struct ViewportPair {
			SharedPtr<RendererViewport> first;
			unsigned int second;
			GLuint min_index;
			GLuint max_index;
		};

		/** Ctor.
		 */
		Renderer();

		void SetupGL( sf::RenderTarget& target ) const;

		void RestoreGL( sf::RenderTarget& target ) const;

		void SortPrimitives();

		void RefreshVBO( const sf::RenderTarget& target );

		sf::Vector2f LoadFont( const sf::Font& font, unsigned int size );

		sf::Vector2f LoadImage( const sf::Image& image, bool force_insert = false );

		void SetupFBO( unsigned int width, unsigned int height );

		void DestroyFBO();

		static SharedPtr<Renderer> m_instance;

		std::vector<Primitive::Ptr> m_primitives;
		std::vector<SharedPtr<RendererViewport> > m_viewports;

		std::vector<ViewportPair> m_viewport_pairs;

		SharedPtr<RendererViewport> m_default_viewport;

		sf::Texture m_texture_atlas;

		GLuint m_frame_buffer;
		GLuint m_frame_buffer_texture;
		GLuint m_frame_buffer_depth;

		GLuint m_display_list;

		typedef std::pair<void*, unsigned int> FontID;

		std::map<const sf::Uint8*, sf::Vector2f> m_atlas_offsets;
		std::map<FontID, sf::Vector2f> m_font_offsets;

		GLuint m_vertex_vbo;
		GLuint m_color_vbo;
		GLuint m_texture_vbo;
		GLuint m_index_vbo;

		GLsizei m_last_vertex_count;
		GLsizei m_last_index_count;

		std::size_t m_vertex_count;
		std::size_t m_index_count;

		float m_alpha_threshold;

		unsigned char m_depth_clear_strategy;

		unsigned char m_vbo_sync_type;

		mutable bool m_depth_alternate_flag;

		mutable bool m_vbo_synced;
		bool m_cull;
		bool m_use_fbo;

		bool m_pseudo_texture_loaded;

		bool m_vbo_supported;
		bool m_fbo_supported;
};

}
