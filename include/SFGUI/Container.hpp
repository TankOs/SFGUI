#pragma once

#include <SFGUI/Widget.hpp>

#include <memory>
#include <vector>

namespace sfg {

/**
 * Base class for container-like widgets.
 */
class SFGUI_API Container : public Widget {
	public:
		typedef std::shared_ptr<Container> Ptr; //!< Shared pointer.
		typedef std::shared_ptr<const Container> PtrConst; //!< Shared pointer.
		typedef std::vector<Widget::Ptr> WidgetsList;

		/** Dtor.
		 */
		virtual ~Container() = default;

		/** Add child.
		 * @param widget Widget to add.
		 */
		void Add( Widget::Ptr widget );

		/** Remove child (from container).
		 * @param widget Widget to remove.
		 */
		void Remove( Widget::Ptr widget );

		/** Remove all children from container.
		 */
		void RemoveAll();

		/** Check if a widget is a child of this container.
		 * @param widget Widget to search for.
		 */
		bool IsChild( Widget::Ptr widget ) const;

		/** Get children.
		 * @return std::list with children.
		 */
		const WidgetsList& GetChildren() const;

		void Refresh() override;

		void HandleEvent( const sf::Event& event ) override;

		/** Used to inform parent that a child has been invalidated
		 * @param child Widget that was invalidated.
		 */
		virtual void HandleChildInvalidate( Widget::PtrConst child ) const;

		/** Handle changing of absolute position
		 */
		void HandleAbsolutePositionChange() override;

	protected:
		/** Handle adding children.
		 * @param child Child widget.
		 * @return true if child was added, false otherwise.
		 */
		virtual bool HandleAdd( Widget::Ptr child );

		/** Handle removing children.
		 * @param child Child widget.
		 */
		virtual void HandleRemove( Widget::Ptr child );

		/** Handle visibility change.
		 */
		void HandleGlobalVisibilityChange() override;

		/** Handle update.
		 */
		void HandleUpdate( float seconds ) override;

		/** Handle hierarchy level change.
		 */
		void HandleSetHierarchyLevel() override;

		/** Handle viewport change.
		 */
		void HandleViewportUpdate() override;

	private:
		WidgetsList m_children;
};

}
