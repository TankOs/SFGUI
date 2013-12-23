#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Renderer.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

namespace sfg {

/** SFGUI Vertex Buffer renderer.
 */
class SFGUI_API VertexBufferRenderer : public Renderer {
	public:
		/** Ctor.
		 */
		VertexBufferRenderer();

		/** Dtor.
		 */
		~VertexBufferRenderer();

		/** Check if using a Vertex Buffer Renderer is supported.
		 * @return true if using a Vertex Buffer Renderer is supported.
		 */
		static bool IsAvailable();

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

		virtual const std::string& GetName() const override;

	protected:
		virtual void InvalidateImpl( unsigned char datasets ) override;
		virtual void InvalidateWindow() override;
		virtual void DisplayImpl() const override;

	private:
		void InvalidateVBO( unsigned char datasets );

		void RefreshVBO();

		void SetupFBO( unsigned int width, unsigned int height );

		void DestroyFBO();

		std::vector<Batch> m_batches;

		GLuint m_frame_buffer;
		GLuint m_frame_buffer_texture;

		GLuint m_display_list;

		GLuint m_vertex_vbo;
		GLuint m_color_vbo;
		GLuint m_texture_vbo;
		GLuint m_index_vbo;

		GLsizei m_last_vertex_count;
		GLsizei m_last_index_count;

		float m_alpha_threshold;

		unsigned char m_vbo_sync_type;

		mutable bool m_vbo_synced;

		bool m_cull;
		bool m_use_fbo;

		bool m_vbo_supported;
		bool m_fbo_supported;

		static bool m_glew_initialized;
};

}
