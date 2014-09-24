#pragma once

#include <SFGUI/Renderer.hpp>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Shader.hpp>
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
class SFGUI_API NonLegacyRenderer : public Renderer {
	public:
		typedef std::shared_ptr<NonLegacyRenderer> Ptr;
		typedef std::shared_ptr<const NonLegacyRenderer> PtrConst;

		/** Create NonLegacyRenderer.
		 * @return NonLegacyRenderer.
		 */
		static Ptr Create();

		/** Dtor.
		 */
		~NonLegacyRenderer();

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
		NonLegacyRenderer();

		void InvalidateImpl( unsigned char datasets ) override;

	private:
		void DisplayImpl() const override;

		void InvalidateVBO( unsigned char datasets );

		void RefreshVBO();

		void SetupFBO( int width, int height );

		void DestroyFBO();

		void SetupVAO();
		void SetupFBOVAO();

		std::vector<sf::Vector2f> m_vertex_data;
		std::vector<sf::Color> m_color_data;
		std::vector<sf::Vector2f> m_texture_data;
		std::vector<unsigned int> m_index_data;

		std::vector<priv::RendererBatch> m_batches;

		unsigned int m_frame_buffer = 0;
		sf::Texture m_frame_buffer_texture;

		unsigned int m_fbo_vbo = 0;
		unsigned int m_fbo_vao = 0;

		sf::Shader m_fbo_shader;
		mutable unsigned int m_fbo_vertex_location = 0;
		mutable unsigned int m_fbo_texture_coordinate_location = 0;

		unsigned int m_vertex_vbo = 0;
		unsigned int m_color_vbo = 0;
		unsigned int m_texture_vbo = 0;
		unsigned int m_index_vbo = 0;

		unsigned int m_vao = 0;

		mutable sf::Shader m_shader;
		mutable unsigned int m_vertex_location = 0;
		mutable unsigned int m_color_location = 0;
		mutable unsigned int m_texture_coordinate_location = 0;

		mutable sf::Vector2i m_previous_window_size;

		int m_last_vertex_count;
		int m_last_index_count;

		unsigned char m_vbo_sync_type;

		mutable bool m_vbo_synced;

		bool m_cull;
		bool m_use_fbo;
};

}
