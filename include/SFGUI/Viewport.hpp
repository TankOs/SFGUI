#pragma once

#include <SFGUI/Bin.hpp>

#include <memory>

namespace sfg {

class Adjustment;

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
		static Ptr Create( std::shared_ptr<Adjustment> horizontal_adjustment, std::shared_ptr<Adjustment> vertical_adjustment );

		/** Get absolute position on virtual screen: always (0,0)
		 * @return Absolute position on virtual screen: always (0,0).
		 */
		sf::Vector2f GetAbsolutePosition() const override;

		/** Get the horizontal Adjustment for this Viewport.
		 * @return Horizontal Adjustment for this Viewport.
		 */
		std::shared_ptr<Adjustment> GetHorizontalAdjustment() const;

		/** Set the horizontal adjustment of this Viewport
		 * @param horizontal_adjustment Horizontal Adjustment
		 */
		void SetHorizontalAdjustment( std::shared_ptr<Adjustment> horizontal_adjustment );

		/** Get the vertical Adjustment for this Viewport.
		 * @return Vertical Adjustment for this Viewport.
		 */
		std::shared_ptr<Adjustment> GetVerticalAdjustment() const;

		/** Set the vertical adjustment of this Viewport
		 * @param vertical_adjustment Vertical Adjustment
		 */
		void SetVerticalAdjustment( std::shared_ptr<Adjustment> vertical_adjustment );

		/** Handle SFML event.
		 * Handle an SFML event and fire proper signals.
		 * @return true when event has been processed (eaten).
		 */
		void HandleEvent( const sf::Event& event ) override;

		const std::string& GetName() const override;

		/** Handle changing of absolute position
		 */
		void HandleAbsolutePositionChange() override;

		/** Get requisition of the child widget of the Viewport if present.
		 * @return requisition of the child widget of the Viewport or (0.f, 0.f) if not present.
		 */
		sf::Vector2f GetChildRequisition();

	protected:
		sf::Vector2f CalculateRequisition() override;
		void HandleSizeChange() override;
		bool HandleAdd( Widget::Ptr child ) override;

		/** Handle viewport change.
		 */
		void HandleViewportUpdate() override;

		std::unique_ptr<RenderQueue> InvalidateImpl() const override;

	private:
		/** Ctor.
		 */
		Viewport();

		void HandleRequisitionChange() override;

		void UpdateView();

		std::shared_ptr<Adjustment> m_horizontal_adjustment;
		std::shared_ptr<Adjustment> m_vertical_adjustment;

		unsigned int m_horizontal_adjustment_signal_serial;
		unsigned int m_vertical_adjustment_signal_serial;

		std::shared_ptr<RendererViewport> m_children_viewport;
};

}
