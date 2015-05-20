#pragma once

#include <SFGUI/Container.hpp>

#include <memory>

namespace sfg {

class Viewport;
class Adjustment;
class Scrollbar;

/** Scrolled Window.
 */
class SFGUI_API ScrolledWindow : public Container {
	public:
		typedef std::shared_ptr<ScrolledWindow> Ptr; //!< Shared pointer.
		typedef std::shared_ptr<const ScrolledWindow> PtrConst; //!< Shared pointer.

		/** Content Placement
		 */
		enum class Placement : char {
			TOP_LEFT = 0, //!< Top left corner.
			TOP_RIGHT, //!< Top right corner.
			BOTTOM_LEFT, //!< Bottom left corner.
			BOTTOM_RIGHT, //!< Bottom right corner.
			DEFAULT = TOP_LEFT
		};

		/** Scrollbar policy.
		 */
		enum ScrollbarPolicy : char {
			HORIZONTAL_ALWAYS = 1 << 0, //!< Always display horizontal scrollbar.
			HORIZONTAL_AUTOMATIC = 1 << 1, //!< Display horizontal scrollbar only when needed.
			HORIZONTAL_NEVER = 1 << 2, //!< Never display horizontal scrollbar.
			VERTICAL_ALWAYS = 1 << 3, //!< Always display vertical scrollbar.
			VERTICAL_AUTOMATIC = 1 << 4, //!< Display vertical scrollbar only when needed.
			VERTICAL_NEVER = 1 << 5, //!< Never display vertical scrollbar.
			DEFAULT = HORIZONTAL_ALWAYS | VERTICAL_ALWAYS
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
		static Ptr Create( std::shared_ptr<Adjustment> horizontal_adjustment, std::shared_ptr<Adjustment> vertical_adjustment );

		const std::string& GetName() const override;

		/** Get the horizontal Adjustment for this Scrolled Window.
		 * @return Horizontal Adjustment for this Scrolled Window.
		 */
		std::shared_ptr<Adjustment> GetHorizontalAdjustment() const;

		/** Set the horizontal Adjustment for this Scrolled Window.
		 * @param adjustment new horizontal Adjustment.
		 */
		void SetHorizontalAdjustment( std::shared_ptr<Adjustment> adjustment );

		/** Get the vertical Adjustment for this Scrolled Window.
		 * @return Vertical Adjustment for this Scrolled Window.
		 */
		std::shared_ptr<Adjustment> GetVerticalAdjustment() const;

		/** Set the vertical Adjustment for this Scrolled Window.
		 * @param adjustment new vertical Adjustment.
		 */
		void SetVerticalAdjustment( std::shared_ptr<Adjustment> adjustment );

		/** Get the scrollbar policy for this Scrolled Window.
		 * @return Scrollbar policy for this Scrolled Window.
		 */
		char GetScrollbarPolicy() const;

		/** Set the scrollbar policy for this Scrolled Window.
		 * @param policy New policy.
		 */
		void SetScrollbarPolicy( char policy );

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
		std::shared_ptr<Viewport> GetViewport() const;

		/** Add child with viewport.
		 * @param widget Widget to add with viewport.
		 */
		void AddWithViewport( Widget::Ptr widget );

		/** Add child.
		 * @param widget Widget to add.
		 */
		void Add( Widget::Ptr widget );

		/** Remove child (from scrolledwindow).
		 * @param widget Widget to remove.
		 */
		void Remove( Widget::Ptr widget );

		/** Used to inform parent that a child has been invalidated
		 * @param child Widget that was invalidated.
		 */
		void HandleChildInvalidate( Widget::PtrConst child ) const override;

	protected:
		std::unique_ptr<RenderQueue> InvalidateImpl() const override;
		sf::Vector2f CalculateRequisition() override;

	private:
		ScrolledWindow( std::shared_ptr<Adjustment> horizontal_adjustment, std::shared_ptr<Adjustment> vertical_adjustment );

		/** Recalculate Adjustments
		 */
		void RecalculateAdjustments() const;

		bool IsHorizontalScrollbarVisible() const;
		bool IsVerticalScrollbarVisible() const;

		/** Recalculate Content Allocation
		 */
		void RecalculateContentAllocation() const;

		void HandleSizeChange() override;
		bool HandleAdd( Widget::Ptr child ) override;

		mutable sf::FloatRect m_content_allocation;

		std::shared_ptr<Scrollbar> m_horizontal_scrollbar;
		std::shared_ptr<Scrollbar> m_vertical_scrollbar;
		std::shared_ptr<Viewport> m_viewport;

		char m_policy;
		Placement m_placement;
};

}
