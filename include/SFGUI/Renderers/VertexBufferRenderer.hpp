#pragma once

#include <SFGUI/Renderer.hpp>

#include <SFML/System/Vector2.hpp>

namespace sf {
class Color;
}

namespace sfg {

namespace priv {
struct RendererBatch;
}

/** SFGUI Vertex Buffer renderer.
 */
class SFGUI_API VertexBufferRenderer : public Renderer {
	public:
		typedef std::shared_ptr<VertexBufferRenderer> Ptr;
		typedef std::shared_ptr<const VertexBufferRenderer> PtrConst;

		/** Create VertexBufferRenderer.
		 * @return VertexBufferRenderer.
		 */
		static Ptr Create();

		/** Dtor.
		 */
		~VertexBufferRenderer();

		/** Check if using a Vertex Buffer Renderer is supported.
		 * @return true if using a Vertex Buffer Renderer is supported.
		 */
		static bool IsAvailable();

		/** Draw the GUI to an sf::Window.
		 * @param target sf::Window to draw to.
		 */
		void Display( sf::Window& target ) const override;

		/** Draw the GUI to an sf::RenderWindow.
		 * @param target sf::RenderWindow to draw to.
		 */
		void Display( sf::RenderWindow& target ) const override;

		/** Draw the GUI to an sf::RenderTexture.
		 * @param target sf::RenderTexture to draw to.
		 */
		void Display( sf::RenderTexture& target ) const override;

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

		const std::string& GetName() const override;

	protected:
		/** Ctor.
		 */
		VertexBufferRenderer();

		void InvalidateImpl( unsigned char datasets ) override;

	private:
		void DisplayImpl() const override;

		void InvalidateVBO( unsigned char datasets );

		void RefreshVBO();

		void SetupFBO( int width, int height );

		void DestroyFBO();

		std::vector<sf::Vector2f> m_vertex_data;
		std::vector<sf::Color> m_color_data;
		std::vector<sf::Vector2f> m_texture_data;
		std::vector<unsigned int> m_index_data;

		std::vector<priv::RendererBatch> m_batches;

		unsigned int m_frame_buffer;
		unsigned int m_frame_buffer_texture;

		unsigned int m_display_list;

		unsigned int m_vertex_vbo;
		unsigned int m_color_vbo;
		unsigned int m_texture_vbo;
		unsigned int m_index_vbo;

		int m_last_vertex_count;
		int m_last_index_count;

		float m_alpha_threshold;

		unsigned char m_vbo_sync_type;

		mutable bool m_vbo_synced;

		bool m_cull;
		bool m_use_fbo;

		bool m_vbo_supported;
		bool m_fbo_supported;
};

}
