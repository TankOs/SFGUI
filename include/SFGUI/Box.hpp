#pragma once
#include <SFGUI/Container.hpp>
#include <list>

namespace sfg {

/** Box sizer.
 * Widgets are arranged horizontally or vertically.
 */
class SFGUI_API Box : public Container {
	public:
		typedef std::shared_ptr<Box>  Ptr; //!< Shared pointer.
		typedef std::shared_ptr<const Box>  PtrConst; //!< Shared pointer.

		/** Box orientation.
		 */
		enum Orientation {
			Horizontal = 0, //!< Arrange horizontally.
			Vertical //!< Arrange vertically.
		};

		/** Create box.
		 * @param orientation Orientation.
		 * @param spacing Spacing = space between widgets.
		 * @return Box.
		 */
		static Ptr Create( Orientation orientation = Horizontal, float spacing = 0.f );

		virtual const std::string& GetName() const;

		/** Add a widget to the box.
		 * @param widget Widget.
		 * @param expand Expand widget to highest possible size.
		 * @param fill Fill calculated size.
		 */
		void Pack( Widget::Ptr widget, bool expand = true, bool fill = true );

		/** Set spacing.
		 * @param spacing Spacing.
		 */
		void SetSpacing( float spacing );

		/** Get spacing.
		 * @return Spacing.
		 */
		float GetSpacing() const;

	protected:
		/** Get requisition.
		 * @return Requisition.
		 */
		sf::Vector2f GetRequisitionImpl() const;

	private:
		struct ChildInfo {
			Widget::Ptr  widget;
			bool         expand;
			bool         fill;

			ChildInfo( Widget::Ptr widget_, bool expand_ = true, bool fill_ = true );
			bool operator==( const ChildInfo& rhs ) const;
		};

		typedef std::list<ChildInfo>  ChildrenCont;

		Box( Orientation orientation = Horizontal, float spacing = 0.f );

		void HandleAdd( Widget::Ptr child );
		void HandleRemove( Widget::Ptr child );
		void HandleSizeAllocate( const sf::FloatRect& old_allocation ) const;

		void AllocateChildren() const;

		Orientation  m_orientation;
		float  m_spacing;
		ChildrenCont  m_children;
};

}
