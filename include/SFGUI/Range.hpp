#pragma once
#include <SFGUI/Config.hpp>
#include <SFGUI/Widget.hpp>
#include <SFGUI/Adjustment.hpp>

namespace sfg {

/** Range widget, base class for scrollbars and sliders.
 */
class SFGUI_API Range : public Widget {
	public:
		typedef boost::shared_ptr<Range>  Ptr; //!< Shared pointer.

		/** Get the Adjustment for this Range widget.
		 * @return Adjustment for this Range widget.
		 */
		Adjustment::Ptr GetAdjustment() const;

		/** Set the Adjustment for this Range widget.
		 * @param adjustment new Adjustment.
		 */
		void SetAdjustment( Adjustment::Ptr adjustment );

		/** Get the value of this Range widget.
		 * @return Value of this Range widget.
		 */
		float GetValue() const;

		/** Set Adjustment valueof this Range widget.
		 * @param new_value new value of this Range widget.
		 */
		void SetValue( float new_value );

		/** Set the increments for this Range widget
		 * @param minor_step new minor step
		 * @param major_step new major step
		 */
		void SetIncrements( float minor_step, float major_step );

		/** Set the range of this Range widget
		 * @param min new minimum
		 * @param max new maximum
		 */
		void SetRange( float min, float max );

	protected:
		/** Ctor.
		 */
		Range();

	private:
		void HandleAdjustmentChange( Adjustment::Ptr adjustment );

		Adjustment::Ptr m_adjustment;
};

}
