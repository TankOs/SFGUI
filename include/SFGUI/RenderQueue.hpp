#pragma once

#include <SFGUI/Config.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <list>

namespace sfg {

/** Simple container for sf::Drawables.
 */
class RenderQueue : public sf::Drawable {
	public:
		/** Add drawable to the queue.
		 * Keep in mind that the queue takes ownership of the drawable.
		 * @param drawable Drawable to add and manage.
		 */
		void Add( sf::Drawable* drawable );

	protected:
		/** Render.
		 * @param target SFML render target.
		 * @param renderer SFML renderer object.
		 */
		void Render( sf::RenderTarget& target, sf::Renderer& renderer ) const;

	private:
		typedef boost::ptr_list<sf::Drawable>  DrawablesList;

		DrawablesList  m_children;
};

}
