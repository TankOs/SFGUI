#pragma once
#include <SFGUI/Config.hpp>
#include <SFGUI/Range.hpp>
#include <SFGUI/SharedPtr.hpp>
#include <SFML/System/Clock.hpp>

namespace sfg {

/** Scrollbar base.
 */
class SFGUI_API Scrollbar : public Range {
	public:
		typedef SharedPtr<Scrollbar> Ptr; //!< Shared pointer.
		typedef SharedPtr<const Scrollbar> PtrConst; //!< Shared pointer.

		/** Create scrollbar widget.
		 * @param orientation Orientation.
		 * @return Scrollbar widget.
		 */
		static Ptr Create( Orientation orientation = HORIZONTAL );

		virtual const std::string& GetName() const;

		/** Create scrollbar widget.
		 * @param adjustment The adjustment to use.
		 * @param orientation Orientation.
		 * @return Scrollbar widget.
		 */
		static Ptr Create( const Adjustment::Ptr& adjustment, Orientation orientation = HORIZONTAL );

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
		RenderQueue* InvalidateImpl() const;
		sf::Vector2f CalculateRequisition();

	private:
		/** Ctor.
		 */
		Scrollbar( const Adjustment::Ptr& adjustment, Orientation orientation = HORIZONTAL );

		virtual void HandleMouseButtonEvent( sf::Mouse::Button button, bool press, int x, int y );
		virtual void HandleMouseMoveEvent( int x, int y );
		virtual void HandleUpdate( float seconds );

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
