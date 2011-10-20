#pragma once
#include <SFGUI/Config.hpp>
#include <SFGUI/Range.hpp>
#include <SFML/System/Clock.hpp>

namespace sfg {

/** Scrollbar base.
 */
class SFGUI_API Scrollbar : public Range {
	public:
		typedef boost::shared_ptr<Scrollbar>  Ptr; //!< Shared pointer.
		typedef boost::shared_ptr<const Scrollbar>  PtrConst; //!< Shared pointer.

		static const float MIN_SLIDER_LENGTH; //!< Minimum trough length.

		/** Scrollbar orientation.
		 */
		enum Orientation {
			Horizontal = 0, //!< Horizontal scrollbar.
			Vertical //!< Vertical scrollbar.
		};

		/** Create scrollbar widget.
		 * @param orientation Orientation.
		 * @return Scrollbar widget.
		 */
		static Ptr Create( Orientation orientation = Horizontal );

		virtual const std::string& GetName() const;

		/** Create scrollbar widget.
		 * @param adjustment The adjustment to use.
		 * @param orientation Orientation.
		 * @return Scrollbar widget.
		 */
		static Ptr Create( Adjustment::Ptr adjustment, Orientation orientation = Horizontal );

		/** Get the orientation of this scrollbar widget
		 * @return orientation of this scrollbar widget
		 */
		Orientation GetOrientation() const;

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
		sf::Drawable* InvalidateImpl();
		sf::Vector2f GetRequisitionImpl() const;

	private:
		/** Ctor.
		 */
		Scrollbar( Adjustment::Ptr adjustment, Orientation orientation = Horizontal );

		bool HandleMouseButtonPress( Widget::Ptr widget, int x, int y, sf::Mouse::Button button );
		bool HandleMouseButtonRelease( Widget::Ptr widget, int x, int y, sf::Mouse::Button button );
		void HandleMouseMove( Widget::Ptr widget, int x, int y );
		void HandleExpose( Widget::Ptr widget, sf::RenderTarget& target );

		Orientation m_orientation;

		bool m_dragging;

		bool m_decrease_pressed;
		bool m_increase_pressed;

		bool m_page_decreasing;
		bool m_page_increasing;

		sf::Clock m_change_timer;

		float m_slider_click_offset;
};

}
