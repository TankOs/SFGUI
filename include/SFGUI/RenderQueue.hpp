#pragma once

#include <SFGUI/Config.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/OpenGL.hpp>
#include <vector>
#include <SFGUI/SharedPtr.hpp>

namespace sfg {

/** Simple container for sf::Drawables.
 */
class SFGUI_API RenderQueue : public sf::Drawable {
	public:
		typedef std::vector< std::pair<sf::Drawable*, sf::FloatRect> > DrawablesVector;

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

		/** Get the AABBs of this queue.
		 * @return AABBs of this queue.
		 */
		sf::IntRect* GetAABBs() const;

		/** Get the amount of AABBs of this queue.
		 * @return Amount of AABBs of this queue.
		 */
		std::size_t GetAABBSize() const;

		/** Set position of the drawable.
		 * @param position Position.
		 */
		void SetPosition( const sf::Vector2f& position );

		/** Compiles AABBs.
		 */
		void Compile();

		/** Get the last View ID this queue was checked against.
		 * @return ID of the last view checked against.
		 */
		unsigned int GetCheckedViewID() const;

		/** Set the last View ID this queue was checked against.
		 * @param id ID of the last view checked against.
		 */
		void SetCheckedViewID( unsigned int id );

		/** Set the View ID used while rendering this queue.
		 * Used to set proper view while flushing the target buffer.
		 * @param id ID of view used while rendering this queue.
		 */
		void SetAssociatedViewID( unsigned int id );

		/** Get the View ID used while rendering this queue.
		 * @return ID of view used while rendering this queue.
		 */
		unsigned int GetAssociatedViewID() const;

		/** Check whether this queue passed culling stage the last check.
		 * @return true if this widget passed culling stage the last check.
		 */
		bool GetCullPass() const;

		/** Set whether this queue passed culling stage the last check.
		 * @param result true if this widget passed culling stage the last check.
		 */
		void SetCullPass( bool result );

		/** Get the drawables and their AABBs in this queue.
		 * @return Drawables and AABBs in this queue.
		 */
		const DrawablesVector& GetDrawables() const;

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

	protected:
		/** Render.
		 * @param target SFML render target.
		 * @param renderer SFML renderer object.
		 */
		void Render( sf::RenderTarget& target, sf::Renderer& renderer ) const;

	private:
		DrawablesVector m_children;

		GLuint m_display_list;
		mutable GLint m_texture_id;

		sf::IntRect* m_compiled_aabbs;
		std::size_t m_compiled_aabbs_size;

		int m_z_order;

		unsigned int m_checked_view_id;
		unsigned int m_associated_view_id;
		bool m_last_cull_passed;

		mutable bool m_display_list_compiled;
};

}
