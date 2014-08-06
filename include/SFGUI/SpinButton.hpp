#pragma once

#include <SFGUI/Entry.hpp>

#include <memory>

namespace sfg {

class Adjustment;

/** SpinButton widget
 */
class SFGUI_API SpinButton : public Entry {
	public:
		typedef std::shared_ptr<SpinButton> Ptr; //!< Shared pointer.
		typedef std::shared_ptr<const SpinButton> PtrConst; //!< Shared pointer.

		/** Create SpinButton.
		 * An adjustment object will be created automatically.
		 * @param minimum Minimum value this SpinButton can be set to.
		 * @param maximum Maximum value this SpinButton can be set to.
		 * @param step Step to increase and decrease value by.
		 * @return SpinButton.
		 */
		static Ptr Create( float minimum, float maximum, float step );

		/** Create SpinButton.
		 * @param adjustment Adjustment to use.
		 * @return SpinButton.
		 */
		static Ptr Create( std::shared_ptr<Adjustment> adjustment );

		const std::string& GetName() const override;

		/** Check if decrease stepper is pressed
		 * @return true if decrease stepper is pressed
		 */
		bool IsDecreaseStepperPressed() const;

		/** Check if increase stepper is pressed
		 * @return true if increase stepper is pressed
		 */
		bool IsIncreaseStepperPressed() const;

		/** Get the Adjustment for this SpinButton widget.
		 * @return Adjustment for this SpinButton widget.
		 */
		std::shared_ptr<Adjustment> GetAdjustment() const;

		/** Set the Adjustment for this SpinButton widget.
		 * @param adjustment New Adjustment.
		 */
		void SetAdjustment( std::shared_ptr<Adjustment> adjustment );

		/** Get the value of this SpinButton widget.
		 * @return Value of this SpinButton widget.
		 */
		float GetValue() const;

		/** Set the value of this SpinButton widget.
		 * @param new_value New value of this SpinButton widget.
		 */
		void SetValue( float new_value );

		/** Get the step value for this SpinButton widget
		 * @return Step value.
		 */
		float GetStep() const;

		/** Set the step value for this SpinButton widget
		 * @param step New step value.
		 */
		void SetStep( float step );

		/** Set the range of this SpinButton widget
		 * @param minimum New minimum.
		 * @param maximum New maximum.
		 */
		void SetRange( float minimum, float maximum );

		/** Get the digits to display after the decimal point.
		 * @return Digits to display after the decimal point.
		 */
		unsigned int GetDigits() const;

		/** Set the digits to display after the decimal point.
		 * @param digits Digits to display after the decimal point.
		 */
		void SetDigits( unsigned int digits );

		/** Configure this SpinButton
		 * @param adjustment New adjustment to use.
		 * @param step New step value to use.
		 * @param digits Digits to display after the decimal point.
		 */
		void Configure( std::shared_ptr<Adjustment> adjustment, float step, unsigned int digits );

		// Signals.
		static Signal::SignalID OnValueChanged; //!< Fired when the value changes.

	protected:
		std::unique_ptr<RenderQueue> InvalidateImpl() const override;
		sf::Vector2f CalculateRequisition() override;

	private:
		/** Ctor.
		 */
		SpinButton();

		void UpdateTextFromAdjustment();
		void UpdateAdjustmentFromText();

		void HandleMouseButtonEvent( sf::Mouse::Button button, bool press, int x, int y ) override;
		void HandleUpdate( float seconds ) override;
		void HandleTextEvent( sf::Uint32 character ) override;
		void HandleKeyEvent( sf::Keyboard::Key key, bool press ) override;
		void HandleSizeChange() override;
		void HandleFocusChange( Widget::Ptr focused_widget ) override;

		std::shared_ptr<Adjustment> m_adjustment;

		float m_elapsed_time;

		unsigned int m_adjustment_signal_serial;
		unsigned int m_digits;

		bool m_decrease_pressed;
		bool m_increase_pressed;

		bool m_repeat_wait;
};

}
