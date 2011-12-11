#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Widget.hpp>
#include <SFGUI/SharedPtr.hpp>
#include <vector>

namespace sfg {

/**
 * Base class for container-like widgets.
 */
class SFGUI_API Container : public Widget {
	public:
		typedef SharedPtr<Container> Ptr; //!< Shared pointer.
		typedef SharedPtr<const Container> PtrConst; //!< Shared pointer.
		typedef std::vector<Widget::Ptr> WidgetsList;

		/** Dtor.
		 */
		virtual ~Container();

		/** Add child.
		 * @param widget Widget to add.
		 */
		void Add( const Widget::Ptr& widget );

		/** Remove child (from container).
		 * @param widget Widget to remove.
		 */
		void Remove( const Widget::Ptr& widget );

		/** Check if a widget is a child of this container.
		 * @param widget Widget to search for.
		 */
		bool IsChild( const Widget::Ptr& widget ) const;

		/** Get children.
		 * @return std::list with children.
		 */
		const WidgetsList& GetChildren() const;

		/** Set border width.
		 * @param width Border width.
		 */
		void SetMargin( float width );

		/** Get border width.
		 * @return Border width.
		 */
		float GetMargin() const;

		void Refresh();

		virtual void HandleEvent( const sf::Event& event );

		/** Used to inform parent that a child has been invalidated
		 * @param child Widget that was invalidated.
		 */
		virtual void HandleChildInvalidate( const Widget::PtrConst& child ) const;

		/** Handle changing of absolute position
		 */
		virtual void HandleAbsolutePositionChange();

	protected:
		/** Constructor.
		 */
		Container();

		/** Handle adding children.
		 * @param child Child widget.
		 */
		virtual void HandleAdd( const Widget::Ptr& child );

		/** Handle removing children.
		 * @param child Child widget.
		 */
		virtual void HandleRemove( const Widget::Ptr& child );

		virtual void HandleExpose( CullingTarget& target ) const;

	private:
		float m_margin;

		WidgetsList m_children;
};

}
