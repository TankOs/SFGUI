#pragma once

#include <SFGUI/Range.hpp>

#include <memory>

namespace sfg {

class Adjustment;

/** Scrollbar base.
 */
class SFGUI_API Scrollbar : public Range {
	public:
		typedef std::shared_ptr<Scrollbar> Ptr; //!< Shared pointer.
		typedef std::shared_ptr<const Scrollbar> PtrConst; //!< Shared pointer.

		/** Create scrollbar widget.
		 * @param orientation Orientation.
		 * @return Scrollbar widget.
		 */
		static Ptr Create( Orientation orientation = Orientation::HORIZONTAL );

		const std::string& GetName() const override;

		/** Create scrollbar widget.
		 * @param adjustment The adjustment to use.
		 * @param orientation Orientation.
		 * @return Scrollbar widget.
		 */
		static Ptr Create( std::shared_ptr<Adjustment> adjustment, Orientation orientation = Orientation::HORIZONTAL );

		/** Get slider rectangle ( position and dimensions )
		 * @return slider rect
		 */
		const sf::FloatRect GetSliderRect() const;

		/** Check if decrease stepper is pressed
		 * @return true if decrease stepper is pressed
		 */
		bool IsDecreaseStepperPressed() const;

		/** Check if increase stepper is pressed
		 * @return true if increase stepper is pressed
		 */
		bool IsIncreaseStepperPressed() const;

	protected:
		std::unique_ptr<RenderQueue> InvalidateImpl() const override;
		sf::Vector2f CalculateRequisition() override;

	private:
		/** Ctor.
		 */
		Scrollbar( Orientation orientation = Orientation::HORIZONTAL );

		void HandleMouseButtonEvent( sf::Mouse::Button button, bool press, int x, int y ) override;
		void HandleMouseMoveEvent( int x, int y ) override;
		void HandleUpdate( float seconds ) override;

		float m_elapsed_time;

		float m_slider_click_offset;

		mutable int m_page_decreasing;
		mutable int m_page_increasing;

		bool m_dragging;

		bool m_decrease_pressed;
		bool m_increase_pressed;

		mutable bool m_repeat_wait;
};

}
