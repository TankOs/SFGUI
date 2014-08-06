#pragma once

#include <SFGUI/Renderer.hpp>

#include <SFML/System/Vector2.hpp>

namespace sf {
class Color;
}

namespace sfg {

namespace priv {
class RendererBatch;
}

/** SFGUI Vertex Array renderer.
 */
class SFGUI_API VertexArrayRenderer : public Renderer {
	public:
		typedef std::shared_ptr<VertexArrayRenderer> Ptr;
		typedef std::shared_ptr<const VertexArrayRenderer> PtrConst;

		/** Create VertexArrayRenderer.
		 * @return VertexArrayRenderer.
		 */
		static Ptr Create();

		/** Enable and select alpha testing threshold.
		 * @param alpha_threshold Threshold at which fragments will get discarded if their alpha value is less than or equal to. Set to 0.f to disable.
		 */
		void TuneAlphaThreshold( float alpha_threshold );

		/** Enable or disable CPU driven face culling.
		 * @param enable true to enable, false to disable.
		 */
		void TuneCull( bool enable );

		const std::string& GetName() const override;

	protected:
		/** Ctor.
		 */
		VertexArrayRenderer();

		void InvalidateImpl( unsigned char datasets ) override;
		void DisplayImpl() const override;

	private:
		void RefreshArray();

		std::vector<sf::Vector2f> m_vertex_data;
		std::vector<sf::Color> m_color_data;
		std::vector<sf::Vector2f> m_texture_data;
		std::vector<int> m_index_data;

		std::vector<priv::RendererBatch> m_batches;

		int m_last_vertex_count;
		int m_last_index_count;

		float m_alpha_threshold;

		mutable bool m_dirty;

		bool m_cull;
};

}
