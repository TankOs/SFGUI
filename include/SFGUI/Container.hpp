#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Widget.hpp>
#include <list>

namespace sfg {

/**
 * Base class for container-like widgets.
 */
class SFGUI_API Container : public Widget {
	public:
		typedef std::shared_ptr<Container>  Ptr; //!< Shared pointer.
		typedef std::shared_ptr<const Container>  PtrConst; //!< Shared pointer.
		typedef std::list<Widget::Ptr>  WidgetsList;

		/** Pack options.
		 */
		enum PackOptions {
			EXPAND = 1 << 0, ///< Expand widget to maximum possible size.
			FILL = 1 << 1 ///< Fill widget to allocated size.
		};

		/** Dtor.
		 */
		virtual ~Container();

		/** Add child.
		 * @param widget Widget to add.
		 */
		void Add( Widget::Ptr widget );

		/** Remove child (from container).
		 * @param widget Widget to remove.
		 */
		void Remove( Widget::Ptr widget );

		/** Check if a widget is a child of this container.
		 * @param widget Widget to search for.
		 */
		bool IsChild( Widget::Ptr widget ) const;

		/** Get children.
		 * @return std::list with children.
		 */
		const WidgetsList& GetChildren() const;

		/** Set border width.
		 * @param width Border width.
		 */
		void SetBorderWidth( float width );

		/** Get border width.
		 * @return Border width.
		 */
		float GetBorderWidth() const;

		void Refresh() const;

		virtual void HandleEvent( const sf::Event& event );

		/** Used to inform parent that a child has been invalidated
		 * @param child Widget that was invalidated.
		 */
		virtual void HandleChildInvalidate( Widget::PtrConst child ) const;

		/** Handle changing of absolute position
		 */
		virtual void HandleAbsolutePositionChange() const;

	protected:
		/** Constructor.
		 */
		Container();

		/** Handle adding children.
		 * @param child Child widget.
		 */
		virtual void HandleAdd( Widget::Ptr child );

		/** Handle removing children.
		 * @param child Child widget.
		 */
		virtual void HandleRemove( Widget::Ptr child );

		virtual void HandleExpose( CullingTarget& target ) const;

	private:
		float m_border_width;

		WidgetsList m_children;

};

}
