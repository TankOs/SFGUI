#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Renderer.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

namespace sfg {

/** SFGUI Vertex Array renderer.
 */
class SFGUI_API VertexArrayRenderer : public Renderer {
	public:
		/** Ctor.
		 */
		VertexArrayRenderer();

		/** Enable and select alpha testing threshold.
		 * @param alpha_threshold Threshold at which fragments will get discarded if their alpha value is less than or equal to. Set to 0.f to disable.
		 */
		void TuneAlphaThreshold( float alpha_threshold );

		/** Enable or disable CPU driven face culling.
		 * @param enable true to enable, false to disable.
		 */
		void TuneCull( bool enable );

		virtual const std::string& GetName() const override;

	protected:
		virtual void InvalidateImpl( unsigned char datasets ) override;
		virtual void DisplayImpl() const override;

	private:
		void RefreshArray();

		std::vector<sf::Vector2f> m_vertex_data;
		std::vector<sf::Color> m_color_data;
		std::vector<sf::Vector2f> m_texture_data;
		std::vector<GLuint> m_index_data;

		std::vector<Batch> m_batches;

		GLsizei m_last_vertex_count;
		GLsizei m_last_index_count;

		float m_alpha_threshold;

		mutable bool m_dirty;

		bool m_cull;
};

}
