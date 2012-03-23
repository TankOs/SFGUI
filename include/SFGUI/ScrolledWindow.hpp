#pragma once
#include <SFGUI/Bin.hpp>
#include <SFGUI/Adjustment.hpp>
#include <SFGUI/Scrollbar.hpp>
#include <SFGUI/Viewport.hpp>
#include <SFGUI/SharedPtr.hpp>

namespace sfg {

/** Scrolled Window.
 */
class SFGUI_API ScrolledWindow : public Container {
	public:
		typedef SharedPtr<ScrolledWindow> Ptr; //!< Shared pointer.
		typedef SharedPtr<const ScrolledWindow> PtrConst; //!< Shared pointer.

		/** Content Placement
		 */
		enum Placement {
			TOP = 1 << 0,
			BOTTOM = 1 << 1,
			LEFT = 1 << 2,
			RIGHT = 1 << 3,
			TOP_LEFT = TOP | LEFT, //!< Top left corner.
			TOP_RIGHT = TOP | RIGHT, //!< Top right corner.
			BOTTOM_LEFT = BOTTOM | LEFT, //!< Bottom left corner.
			BOTTOM_RIGHT = BOTTOM | RIGHT, //!< Bottom right corner.
			DEFAULT_PLACEMENT = TOP_LEFT
		};

		/** Scrollbar policy.
		 */
		enum ScrollbarPolicy {
			HORIZONTAL_ALWAYS = 1 << 0, //!< Always display horizontal scrollbar.
			HORIZONTAL_AUTOMATIC = 1 << 1, //!< Display horizontal scrollbar only when needed.
			HORIZONTAL_NEVER = 1 << 2, //!< Never display horizontal scrollbar.
			VERTICAL_ALWAYS = 1 << 3, //!< Always display vertical scrollbar.
			VERTICAL_AUTOMATIC = 1 << 4, //!< Display vertical scrollbar only when needed.
			VERTICAL_NEVER = 1 << 5, //!< Never display vertical scrollbar.
			DEFAULT_POLICY = HORIZONTAL_ALWAYS | VERTICAL_ALWAYS
		};

		/** Create scrolled window.
		 * @return ScrolledWindow.
		 */
		static Ptr Create();

		/** Create scrolled window with given Adjustments.
		 * @param horizontal_adjustment Horizontal adjustment.
		 * @param vertical_adjustment Vertical adjustment.
		 * @return ScrolledWindow.
		 */
		static Ptr Create( const Adjustment::Ptr& horizontal_adjustment, const Adjustment::Ptr& vertical_adjustment );

		virtual const std::string& GetName() const;

		/** Get the horizontal Adjustment for this Scrolled Window.
		 * @return Horizontal Adjustment for this Scrolled Window.
		 */
		const Adjustment::Ptr& GetHorizontalAdjustment() const;

		/** Set the horizontal Adjustment for this Scrolled Window.
		 * @param adjustment new horizontal Adjustment.
		 */
		void SetHorizontalAdjustment( const Adjustment::Ptr& adjustment );

		/** Get the vertical Adjustment for this Scrolled Window.
		 * @return Vertical Adjustment for this Scrolled Window.
		 */
		const Adjustment::Ptr& GetVerticalAdjustment() const;

		/** Set the vertical Adjustment for this Scrolled Window.
		 * @param adjustment new vertical Adjustment.
		 */
		void SetVerticalAdjustment( const Adjustment::Ptr& adjustment );

		/** Get the scrollbar policy for this Scrolled Window.
		 * @return Scrollbar policy for this Scrolled Window.
		 */
		unsigned char GetScrollbarPolicy() const;

		/** Set the scrollbar policy for this Scrolled Window.
		 * @param policy New policy.
		 */
		void SetScrollbarPolicy( unsigned char policy );

		/** Set where the content is placed and hence where the scrollbars will appear.
		 * @param placement new ScrollbarPolicyPair.
		 */
		void SetPlacement( Placement placement );

		/** Get the allocation of the content area of this Scrolled Window
		 * @return Allocation of the content area of this Scrolled Window
		 */
		const sf::FloatRect& GetContentAllocation() const;

		/** Get child viewport.
		 * @return Child viewport.
		 */
		Viewport::Ptr GetViewport() const;

		/** Add child with viewport.
		 * @param widget Widget to add with viewport.
		 */
		void AddWithViewport( const Widget::Ptr& widget );

		/** Add child.
		 * @param widget Widget to add.
		 */
		void Add( const Widget::Ptr& widget );

		/** Remove child (from scrolledwindow).
		 * @param widget Widget to remove.
		 */
		void Remove( const Widget::Ptr& widget );

		/** Used to inform parent that a child has been invalidated
		 * @param child Widget that was invalidated.
		 */
		virtual void HandleChildInvalidate( const Widget::PtrConst& child ) const;

	protected:
		RenderQueue* InvalidateImpl() const;
		sf::Vector2f CalculateRequisition();

	private:
		ScrolledWindow( const Adjustment::Ptr& horizontal_adjustment, const Adjustment::Ptr& vertical_adjustment );

		/** Recalculate Adjustments
		 */
		void RecalculateAdjustments() const;

		bool IsHorizontalScrollbarVisible() const;
		bool IsVerticalScrollbarVisible() const;

		/** Recalculate Content Allocation
		 */
		void RecalculateContentAllocation() const;

		void HandleSizeChange();
		void HandleAdd( const Widget::Ptr& child );

		mutable sf::FloatRect m_content_allocation;

		Scrollbar::Ptr m_horizontal_scrollbar;
		Scrollbar::Ptr m_vertical_scrollbar;

		unsigned char m_policy;
		unsigned char m_placement;
};

}
