#pragma once

#include <SFGUI/Config.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <vector>

namespace sfg {

/** Simple container for sf::Drawables.
 */
class SFGUI_API RenderQueue : public sf::Drawable {
	public:
		/** Dtor.
		 */
		~RenderQueue();

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
		typedef std::vector<sf::Drawable*>  DrawablesVector;

		DrawablesVector m_children;
};

}
