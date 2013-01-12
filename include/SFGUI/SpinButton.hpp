#pragma once
#include <SFGUI/Entry.hpp>
#include <SFGUI/Adjustment.hpp>
#include <SFGUI/SharedPtr.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Clock.hpp>

namespace sfg {

/** SpinButton widget
 */
class SFGUI_API SpinButton : public Entry {
	public:
		typedef SharedPtr<SpinButton> Ptr; //!< Shared pointer.
		typedef SharedPtr<const SpinButton> PtrConst; //!< Shared pointer.

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
		static Ptr Create( const Adjustment::Ptr& adjustment );

		virtual const std::string& GetName() const;

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
		const Adjustment::Ptr& GetAdjustment() const;

		/** Set the Adjustment for this SpinButton widget.
		 * @param adjustment New Adjustment.
		 */
		void SetAdjustment( const Adjustment::Ptr& adjustment );

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
		 * @param min New minimum.
		 * @param max New maximum.
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
		void Configure( const Adjustment::Ptr& adjustment, float step, unsigned int digits );

		// Signals.
		static Signal::SignalID OnValueChanged; //!< Fired when the value changes.

	protected:
		RenderQueue* InvalidateImpl() const;
		sf::Vector2f CalculateRequisition();

	private:
		/** Ctor.
		 */
		SpinButton( const Adjustment::Ptr& adjustment );

		void UpdateTextFromAdjustment();
		void UpdateAdjustmentFromText();

		virtual void HandleMouseButtonEvent( sf::Mouse::Button button, bool press, int x, int y );
		virtual void HandleUpdate( float seconds );
		virtual void HandleTextEvent( sf::Uint32 character );
		virtual void HandleKeyEvent( sf::Keyboard::Key key, bool press );
		virtual void HandleSizeChange();
		virtual void HandleFocusChange( const Widget::Ptr& focused_widget );

		Adjustment::Ptr m_adjustment;

		float m_elapsed_time;

		unsigned int m_adjustment_signal_serial;
		unsigned int m_digits;

		bool m_decrease_pressed;
		bool m_increase_pressed;

		bool m_repeat_wait;
};

}
