#pragma once
#include <SFGUI/Bin.hpp>
#include <SFGUI/Adjustment.hpp>
#include <SFGUI/Scrollbar.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

namespace sfg {

/** Scrolled Window.
 */
class SFGUI_API ScrolledWindow : public Container {
	public:
		typedef boost::shared_ptr<ScrolledWindow>  Ptr; //!< Shared pointer.
		typedef boost::shared_ptr<const ScrolledWindow>  PtrConst; //!< Shared pointer.

		/** Content Placement
		 */
		enum Placement {
			Top = 1 << 0,
			Bottom = 1 << 1,
			Left = 1 << 2,
			Right = 1 << 3,
			TopLeft = Top | Left, //!< Top left corner.
			TopRight = Top | Right, //!< Top right corner.
			BottomLeft = Bottom | Left, //!< Bottom left corner.
			BottomRight = Bottom | Right, //!< Bottom right corner.
			PlacementDefault = TopLeft
		};

		/** Scrollbar policy.
		 */
		enum ScrollbarPolicy {
			HorizontalAlways = 1 << 0, //!< Always display horizontal scrollbar.
			HorizontalAutomatic = 1 << 1, //!< Display horizontal scrollbar only when needed.
			HorizontalNever = 1 << 2, //!< Never display horizontal scrollbar.
			VerticalAlways = 1 << 3, //!< Always display vertical scrollbar.
			VerticalAutomatic = 1 << 4, //!< Display vertical scrollbar only when needed.
			VerticalNever = 1 << 5, //!< Never display vertical scrollbar.
			PolicyDefault = HorizontalAlways | VerticalAlways
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
		static Ptr Create( Adjustment::Ptr horizontal_adjustment, Adjustment::Ptr vertical_adjustment );

		virtual const std::string& GetName() const;

		/** Get the horizontal Adjustment for this Scrolled Window.
		 * @return Horizontal Adjustment for this Scrolled Window.
		 */
		Adjustment::Ptr GetHorizontalAdjustment() const;

		/** Set the horizontal Adjustment for this Scrolled Window.
		 * @param adjustment new horizontal Adjustment.
		 */
		void SetHorizontalAdjustment( Adjustment::Ptr adjustment );

		/** Get the vertical Adjustment for this Scrolled Window.
		 * @return Vertical Adjustment for this Scrolled Window.
		 */
		Adjustment::Ptr GetVerticalAdjustment() const;

		/** Set the vertical Adjustment for this Scrolled Window.
		 * @param adjustment new vertical Adjustment.
		 */
		void SetVerticalAdjustment( Adjustment::Ptr adjustment );

		/** Get the scrollbar policy for this Scrolled Window.
		 * @return Scrollbar policy for this Scrolled Window.
		 */
		int GetScrollbarPolicy() const;

		/** Set the scrollbar policy for this Scrolled Window.
		 * @param policy New policy.
		 */
		void SetScrollbarPolicy( int policy );

		/** Set where the content is placed and hence where the scrollbars will appear.
		 * @param adjustment new ScrollbarPolicyPair.
		 */
		void SetPlacement( Placement placement );

		/** Get the allocation of the content area of this Scrolled Window
		 * @return Allocation of the content area of this Scrolled Window
		 */
		const sf::FloatRect& GetContentAllocation() const;

		/** Get child widget.
		 * @return Child widget.
		 */
		Widget::Ptr GetChild() const;

		/** Handle SFML event.
		 * Handle an SFML event and fire proper signals.
		 * @return true when event has been processed (eaten).
		 */
		virtual HandleEventResult HandleEvent( const sf::Event& event );

	protected:
		sf::Drawable* InvalidateImpl();
		sf::Vector2f GetRequisitionImpl() const;

	private:
		ScrolledWindow( Adjustment::Ptr horizontal_adjustment, Adjustment::Ptr vertical_adjustment );

		/** Recalculate Adjustments
		 */
		void RecalculateAdjustments() const;

		bool IsHorizontalScrollbarVisible() const;
		bool IsVerticalScrollbarVisible() const;

		/** Recalculate Content Allocation
		 */
		void RecalculateContentAllocation();

		void HandleSizeAllocate();
		void HandleExpose();
		void HandleAdd();

		Scrollbar::Ptr m_horizontal_scrollbar;
		Scrollbar::Ptr m_vertical_scrollbar;

		int m_policy;
		int m_placement;

		sf::RenderTexture m_render_texture;
		sf::Sprite m_sprite;

		mutable bool m_recalc_adjustments;

		sf::FloatRect m_content_allocation;
		mutable bool m_recalc_content_allocation;
};

}
