#pragma once

#include <SFGUI/Config.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/OpenGL.hpp>
#include <vector>
#include <memory>

namespace sfg {

/** Simple container for sf::Drawables.
 */
class SFGUI_API RenderQueue : public sf::Drawable {
	public:
		typedef std::vector< std::pair<sf::Drawable*, sf::FloatRect> >  DrawablesVector;

		/** Ctor.
		 */
		RenderQueue();

		/** Dtor.
		 */
		~RenderQueue();

		/** Add shape to the queue.
		 * Keep in mind that the queue takes ownership of the shape.
		 * @param shape Shape to add and manage.
		 */
		void Add( sf::Shape* shape );

		/** Add sprite to the queue.
		 * Keep in mind that the queue takes ownership of the sprite.
		 * @param sprite Sprite to add and manage.
		 */
		void Add( sf::Sprite* sprite );

		/** Add text to the queue.
		 * Keep in mind that the queue takes ownership of the text.
		 * @param text Text to add and manage.
		 */
		void Add( sf::Text* text );

		/** Add queue to this queue.
		 * Keep in mind that this queue takes ownership of the queue.
		 * @param queue Queue to add and manage.
		 */
		void Add( RenderQueue* queue );

		/** Get the Drawables in this queue.
		 * @return Drawables in this queue.
		 */
		const DrawablesVector& GetDrawables() const;

		/** Set position of the drawable
		 * @param position Position
		 */
		void SetPosition( const sf::Vector2f& position );

	protected:
		/** Render.
		 * @param target SFML render target.
		 * @param renderer SFML renderer object.
		 */
		void Render( sf::RenderTarget& target, sf::Renderer& renderer ) const;

	private:
		DrawablesVector m_children;

		GLuint m_display_list;

		mutable bool m_display_list_compiled;
};

}
