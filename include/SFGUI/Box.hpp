#pragma once

#include <SFGUI/Container.hpp>

#include <memory>
#include <deque>

namespace sfg {

/** Box sizer.
 * Widgets are arranged horizontally or vertically.
 */
class SFGUI_API Box : public Container {
	public:
		typedef std::shared_ptr<Box> Ptr; //!< Shared pointer.
		typedef std::shared_ptr<const Box> PtrConst; //!< Shared pointer.

		/** Box orientation.
		 */
		enum class Orientation : char {
			HORIZONTAL = 0, //!< Arrange horizontally.
			VERTICAL //!< Arrange vertically.
		};

		/** Create box.
		 * @param orientation Orientation.
		 * @param spacing Spacing = space between widgets.
		 * @return Box.
		 */
		static Ptr Create( Orientation orientation = Orientation::HORIZONTAL, float spacing = 0.f );

		const std::string& GetName() const override;

		/** Add a widget to the end of the box.
		 * Alias to PackEnd(...) for backward compatibility.
		 * @param widget Widget.
		 * @param expand Expand widget to highest possible size.
		 * @param fill Fill calculated size.
		 */
		void Pack( Widget::Ptr widget, bool expand = true, bool fill = true );

		/** Add a widget to the start of the box.
		 * @param widget Widget.
		 * @param expand Expand widget to highest possible size.
		 * @param fill Fill calculated size.
		 */
		void PackStart( Widget::Ptr widget, bool expand = true, bool fill = true );

		/** Add a widget to the end of the box.
		 * @param widget Widget.
		 * @param expand Expand widget to highest possible size.
		 * @param fill Fill calculated size.
		 */
		void PackEnd( Widget::Ptr widget, bool expand = true, bool fill = true );

		/** Reorder a child to a new location within the box.
		 * @param widget Child to reorder.
		 * @param position Position to reorder to. 0 for the start, size - 1 or greater for the end.
		 */
		void ReorderChild( Widget::Ptr widget, std::size_t position );

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
		sf::Vector2f CalculateRequisition() override;

	private:
		struct ChildInfo {
			Widget* widget;
			bool expand;
			bool fill;

			ChildInfo( Widget::Ptr widget_, bool expand_ = true, bool fill_ = true );
			bool operator==( const ChildInfo& rhs ) const;
		};

		typedef std::deque<ChildInfo> ChildrenCont;

		Box( Orientation orientation = Orientation::HORIZONTAL, float spacing = 0.f );

		bool HandleAdd( Widget::Ptr child ) override;
		void HandleRemove( Widget::Ptr child ) override;
		void HandleSizeChange() override;
		void HandleRequisitionChange() override;

		void AllocateChildren() const;
		bool IsChildInteresting( Widget* child ) const;

		ChildrenCont m_box_children;
		float m_spacing;
		Orientation m_orientation;
};

}
