#pragma once
#include <SFGUI/Config.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/View.hpp>

/** The Scissor class provides an interface for using glScissor() as an SFML drawable.
 */
class SFGUI_API Scissor : public sf::Drawable {
	public:
		/** Ctor.
		 * @param enable Enable or disable scissor.
		 * @param viewport Viewport.
		 */
		Scissor( bool enable, const sf::FloatRect& viewport = sf::FloatRect( 0, 0, 0, 0 ) );

	protected:
		/** Render.
		 * @param target SFML render target.
		 * @param renderer SFML renderer object.
		 */
		void Render( sf::RenderTarget& target, sf::Renderer& renderer ) const;

	private:
		sf::FloatRect  m_viewport;
		bool  m_enable;
		mutable sf::View  m_view;
		mutable const sf::View  *m_prev_view;
};
