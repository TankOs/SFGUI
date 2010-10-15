#pragma once
#include <SFGUI/Container.hpp>
#include <set>

namespace sfg {

/** Box sizer.
 * Widgets are arranged horizontally or vertically.
 */
class Box : public Container {
	public:
		typedef boost::shared_ptr<Box>  Ptr; //!< Shared pointer.

		/** Box orientation.
		 */
		enum Orientation {
			Horizontal = 0, //!< Arrange horizontally.
			Vertical //!< Arrange vertically.
		};

		/** Create box.
		 * @param orientation Orientation.
		 * @return Box.
		 */
		static Ptr Create( Orientation orientation = Horizontal );

		/** Add a widget to the box.
		 * @param widget Widget.
		 * @param expand Expand widget to highest possible size.
		 * @param fill Fill calculated size.
		 */
		void Pack( Widget::Ptr widget, bool expand = true, bool fill = true );

		virtual void QueueResize( Widget::Ptr widget );

	private:
		struct ChildInfo {
			Widget::Ptr  widget;
			bool         expand;
			bool         fill;

			ChildInfo( Widget::Ptr widget_, bool expand_ = true, bool fill_ = true );
			bool operator<( const ChildInfo& rhs ) const;
		};

		typedef std::set<ChildInfo>  ChildrenCont;

		Box( Orientation orientation = Horizontal );

		sf::Vector2f AllocateChildrenSizes();

		void HandleAdd( Widget::Ptr widget, Widget::Ptr child );
		void HandleRemove( Widget::Ptr widget, Widget::Ptr child );
		void HandleSizeAllocate( Widget::Ptr widget, const sf::FloatRect& oldallocation );

		Orientation  m_orientation;
		ChildrenCont  m_children;
};

}
