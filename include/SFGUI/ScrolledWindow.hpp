#pragma once
#include <SFGUI/Bin.hpp>
#include <SFGUI/Adjustment.hpp>
#include <SFGUI/Scrollbar.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderImage.hpp>

namespace sfg {

/** Scrolled Window.
 */
class SFGUI_API ScrolledWindow : public Bin {
	public:
		typedef boost::shared_ptr<ScrolledWindow>  Ptr; //!< Shared pointer.

		/** Content Placement
		 */
		enum Placement {
			TopLeft = 0, //!< Top left corner.
			TopRight, //!< Top right corner.
			BottomLeft, //!< Bottom left corner.
			BottomRight //!< Bottom right corner.
		};

		/** Scrollbar policy.
		 */
		enum ScrollbarPolicy {
			Always = 0, //!< Always display scrollbar.
			Automatic, //!< Display scrollbar only when needed.
			Never //!< Never display scrollbar.
		};

		/** Scrollbar policy pair.
		 */
		struct ScrollbarPolicyPair {
			ScrollbarPolicy horizontal_policy;
			ScrollbarPolicy vertical_policy;
		};

		/** Create scrolled window.
		 * @return ScrolledWindow.
		 */
		static Ptr Create();

		/** Create scrolled window.
		 * @param horizontal_adjustment Horizontal adjustment.
		 * @param vertical_adjustment Vertical adjustment.
		 * @return ScrolledWindow.
		 */
		static Ptr Create( Adjustment::Ptr horizontal_adjustment, Adjustment::Ptr vertical_adjustment );

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

		/** Get the scrollbar policies for this Scrolled Window.
		 * @return ScrollbarPolicyPair for this Scrolled Window.
		 */
		const ScrollbarPolicyPair& GetScrollbarPolicies() const;

		/** Set the scrollbar policies for this Scrolled Window.
		 * @param adjustment new ScrollbarPolicyPair.
		 */
		void SetScrollbarPolicies( const ScrollbarPolicyPair& policies );

		/** Set where the content is placed and hence where the scrollbars will appear.
		 * @param adjustment new ScrollbarPolicyPair.
		 */
		void SetPlacement( Placement placement );
	protected:
		sf::Vector2f GetRequisitionImpl() const;

	private:
		ScrolledWindow( Adjustment::Ptr horizontal_adjustment, Adjustment::Ptr vertical_adjustment );

		void HandleSizeAllocate( Widget::Ptr widget, const sf::FloatRect& oldallocation );
		void HandlePositionChange( Widget::Ptr widget, const sf::FloatRect& oldallocation );
		void HandleExpose( Widget::Ptr widget, sf::RenderTarget& target );

		Scrollbar::Ptr m_horizontal_scrollbar;
		Scrollbar::Ptr m_vertical_scrollbar;

		ScrollbarPolicyPair m_policies;
		Placement m_placement;

		sf::RenderImage m_render_image;
		sf::Sprite m_sprite;
};

}
