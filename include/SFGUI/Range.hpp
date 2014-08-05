#pragma once

#include <SFGUI/Widget.hpp>

#include <memory>

namespace sfg {

class Adjustment;

/** Range widget, base class for scrollbars and sliders.
 */
class SFGUI_API Range : public Widget {
	public:
		typedef std::shared_ptr<Range> Ptr; //!< Shared pointer.
		typedef std::shared_ptr<const Range> PtrConst; //!< Shared pointer.

		/** Orientation.
		 */
		enum class Orientation : char {
			HORIZONTAL = 0, //!< Horizontal range widget.
			VERTICAL //!< Vertical range widget.
		};

		/** Get the Adjustment for this Range widget.
		 * @return Adjustment for this Range widget.
		 */
		std::shared_ptr<Adjustment> GetAdjustment() const;

		/** Set the Adjustment for this Range widget.
		 * @param adjustment new Adjustment.
		 */
		void SetAdjustment( std::shared_ptr<Adjustment> adjustment );

		/** Get the value of this Range widget.
		 * @return Value of this Range widget.
		 */
		float GetValue() const;

		/** Set the value of this Range widget.
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

		/** Get the orientation of this range widget
		 * @return orientation of this range widget
		 */
		Orientation GetOrientation() const;

	protected:
		/** Ctor.
		 */
		Range( Orientation orientation = Orientation::HORIZONTAL );

	private:
		void HandleAdjustmentChange();

		std::shared_ptr<Adjustment> m_adjustment;

		unsigned int m_change_connection;

		Orientation m_orientation;
};

}
