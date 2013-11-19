#pragma once
#include <SFGUI/Config.hpp>
#include <SFGUI/Bin.hpp>
#include <SFGUI/Adjustment.hpp>
#include <memory>

namespace sfg {

/** Viewport.
 */
class SFGUI_API Viewport : public Bin {
	public:
		typedef std::shared_ptr<Viewport> Ptr; //!< Shared pointer.
		typedef std::shared_ptr<const Viewport> PtrConst; //!< Shared pointer.

		/** Create viewport.
		 * @return Viewport.
		 */
		static Ptr Create();

		/** Create viewport with the provided adjustments.
		 * @param horizontal_adjustment Horizontal adjustment.
		 * @param vertical_adjustment Vertical adjustment.
		 * @return Viewport.
		 */
		static Ptr Create( Adjustment::Ptr horizontal_adjustment, Adjustment::Ptr vertical_adjustment );

		/** Get absolute position on virtual screen: always (0,0)
		 * @return Absolute position on virtual screen: always (0,0).
		 */
		virtual sf::Vector2f GetAbsolutePosition() const override;

		/** Get the horizontal Adjustment for this Viewport.
		 * @return Horizontal Adjustment for this Viewport.
		 */
		Adjustment::Ptr GetHorizontalAdjustment() const;

		/** Set the horizontal adjustment of this Viewport
		 * @param horizontal_adjustment Horizontal Adjustment
		 */
		void SetHorizontalAdjustment( Adjustment::Ptr horizontal_adjustment );

		/** Get the vertical Adjustment for this Viewport.
		 * @return Vertical Adjustment for this Viewport.
		 */
		Adjustment::Ptr GetVerticalAdjustment() const;

		/** Set the vertical adjustment of this Viewport
		 * @param vertical_adjustment Vertical Adjustment
		 */
		void SetVerticalAdjustment( Adjustment::Ptr vertical_adjustment );

		/** Handle SFML event.
		 * Handle an SFML event and fire proper signals.
		 * @return true when event has been processed (eaten).
		 */
		virtual void HandleEvent( const sf::Event& event ) override;

		virtual const std::string& GetName() const override;

		/** Handle changing of absolute position
		 */
		virtual void HandleAbsolutePositionChange() override;

		/** Get requisition of the child widget of the Viewport if present.
		 * @return requisition of the child widget of the Viewport or (0.f, 0.f) if not present.
		 */
		sf::Vector2f GetChildRequisition();

	protected:
		sf::Vector2f CalculateRequisition() override;
		virtual void HandleSizeChange() override;
		virtual void HandleAdd( Widget::Ptr child ) override;

		/** Handle viewport change.
		 */
		virtual void HandleViewportUpdate() override;

		virtual std::unique_ptr<RenderQueue> InvalidateImpl() const override;

	private:
		Viewport( Adjustment::Ptr horizontal_adjustment, Adjustment::Ptr vertical_adjustment );

		void HandleRequisitionChange();

		void UpdateView();

		Adjustment::Ptr m_horizontal_adjustment;
		Adjustment::Ptr m_vertical_adjustment;

		std::shared_ptr<RendererViewport> m_children_viewport;
};

}
