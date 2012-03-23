#pragma once
#include <SFGUI/Container.hpp>
#include <SFGUI/SharedPtr.hpp>
#include <list>

namespace sfg {

/** Box sizer.
 * Widgets are arranged horizontally or vertically.
 */
class SFGUI_API Box : public Container {
	public:
		typedef SharedPtr<Box> Ptr; //!< Shared pointer.
		typedef SharedPtr<const Box> PtrConst; //!< Shared pointer.

		/** Box orientation.
		 */
		enum Orientation {
			HORIZONTAL = 0, //!< Arrange horizontally.
			VERTICAL //!< Arrange vertically.
		};

		/** Create box.
		 * @param orientation Orientation.
		 * @param spacing Spacing = space between widgets.
		 * @return Box.
		 */
		static Ptr Create( Orientation orientation = HORIZONTAL, float spacing = 0.f );

		virtual const std::string& GetName() const;

		/** Add a widget to the box.
		 * @param widget Widget.
		 * @param expand Expand widget to highest possible size.
		 * @param fill Fill calculated size.
		 */
		void Pack( const Widget::Ptr& widget, bool expand = true, bool fill = true );

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
		sf::Vector2f CalculateRequisition();

	private:
		struct ChildInfo {
			Widget::Ptr widget;
			bool expand;
			bool fill;

			ChildInfo( const Widget::Ptr& widget_, bool expand_ = true, bool fill_ = true );
			bool operator==( const ChildInfo& rhs ) const;
		};

		typedef std::list<ChildInfo> ChildrenCont;

		Box( Orientation orientation = HORIZONTAL, float spacing = 0.f );

		void HandleAdd( const Widget::Ptr& child );
		void HandleRemove( const Widget::Ptr& child );
		void HandleSizeChange();
		void HandleRequisitionChange();

		void AllocateChildren() const;
		bool IsChildInteresting( const sfg::Widget::PtrConst& child ) const;

		ChildrenCont m_children;
		float m_spacing;
		unsigned char m_orientation;
};

}
