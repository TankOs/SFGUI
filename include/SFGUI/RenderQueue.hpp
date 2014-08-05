#pragma once

#include <SFGUI/Config.hpp>

#include <SFML/System/Vector2.hpp>
#include <vector>
#include <memory>

namespace sf {
class Shape;
class Sprite;
class Text;
class VertexArray;
}

namespace sfg {

class RendererViewport;
class Primitive;

/** Simple container for sf::Drawables.
 */
class SFGUI_API RenderQueue {
	public:
		/** Ctor.
		 */
		RenderQueue();

		/** Dtor.
		 */
		~RenderQueue();

		/** Add queue to this queue.
		 * Keep in mind that this queue takes ownership of the queue.
		 * @param queue Queue to add and manage.
		 */
		void Add( std::unique_ptr<RenderQueue> queue );

		/** Add a primitive to this queue.
		 * @param primitive Primitive to add.
		 */
		void Add( std::shared_ptr<Primitive> primitive );

		/** Get position of the drawable.
		 * @return Position of the drawable.
		 */
		const sf::Vector2f& GetPosition() const;

		/** Set position of the drawable.
		 * @param position Position.
		 */
		void SetPosition( const sf::Vector2f& position );

		/** Get the primitives in this queue.
		 * @return Primitives in this queue.
		 */
		const std::vector<std::shared_ptr<Primitive>>& GetPrimitives() const;

		/** Get the Z layer this object should be rendered in.
		 * Larger values are rendered later. Default: 0.
		 * @return Z layer this object should be rendered in.
		 */
		int GetZOrder() const;

		/** Set the Z layer this object should be rendered in.
		 * Larger values are rendered later. Default: 0.
		 * @param z_order Z layer this object should be rendered in.
		 */
		void SetZOrder( int z_order );

		/** Draw this std::unique_ptr<RenderQueue>.
		 * @param show true to draw.
		 */
		void Show( bool show = true );

		/** Set draw hierarchy level of this std::unique_ptr<RenderQueue>.
		 * @param level Draw hierarchy level of this std::unique_ptr<RenderQueue>.
		 */
		void SetLevel( int level );

		/** Set viewport of this std::unique_ptr<RenderQueue>.
		 * @param viewport New viewport of this std::unique_ptr<RenderQueue>.
		 */
		void SetViewport( std::shared_ptr<RendererViewport> viewport );

		/** Get viewport of this std::unique_ptr<RenderQueue>.
		 * @return Viewport of this std::unique_ptr<RenderQueue>.
		 */
		std::shared_ptr<RendererViewport> GetViewport() const;

	private:
		std::vector<std::shared_ptr<Primitive>> m_primitives;

		sf::Vector2f m_position;
		std::shared_ptr<RendererViewport> m_viewport;

		int m_z_order;
		int m_level;

		bool m_show;
};

}
