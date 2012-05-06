#pragma once
#include <SFGUI/Config.hpp>
#include <SFGUI/Object.hpp>
#include <SFGUI/Signal.hpp>
#include <SFGUI/SharedPtr.hpp>

namespace sfg {

/** Scrollbar/Slider Adjustment.
 */
class SFGUI_API Adjustment : public Object, public EnableSharedFromThis<Adjustment> {
	public:
		typedef SharedPtr<Adjustment> Ptr; //!< Shared pointer.
		typedef SharedPtr<const Adjustment> PtrConst; //!< Shared pointer.

		/** Dtor.
		 */
		~Adjustment();

		/** Create Adjustment.
		 * @param value Current value.
		 * @param lower Minimum value.
		 * @param upper Maximum value.
		 * @param minor_step Minor change value (such as clicking on arrow button).
		 * @param major_step Major change value (such as clicking on the scroll area).
		 * @param page_size Page size (how many entries are visible / slider size).
		 * @return Adjustment.
		 */
		static Ptr Create( float value = .0f, float lower = .0f, float upper = .0f, float minor_step = 1.f, float major_step = 5.f, float page_size = .0f );

		/** Assignment operator
		 * @param adjustment Adjustment whose values are to be assigned to this one.
		 * @return Reference to this Adjustment.
		 */
		Adjustment& operator=( const Adjustment& adjustment );

		/** Get Adjustment value
		 * @return Adjustment value
		 */
		float GetValue() const;

		/** Set Adjustment value
		 * @param new_value new value
		 */
		void SetValue( float new_value );

		/** Get Adjustment minimum value
		 * @return Adjustment minimum value
		 */
		float GetLower() const;

		/** Set Adjustment minimum value
		 * @param new_lower new minimum value
		 */
		void SetLower( float new_lower );

		/** Get Adjustment maximum value
		 * @return Adjustment maximum value
		 */
		float GetUpper() const;

		/** Set Adjustment maximum value
		 * @param new_upper new maximum value
		 */
		void SetUpper( float new_upper );

		/** Get Adjustment minor step
		 * @return Adjustment minor step
		 */
		float GetMinorStep() const;

		/** Set Adjustment minor step
		 * @param new_minor_step new minor step
		 */
		void SetMinorStep( float new_minor_step );

		/** Get Adjustment major step
		 * @return Adjustment major step
		 */
		float GetMajorStep() const;

		/** Set Adjustment major step
		 * @param new_major_step new major step
		 */
		void SetMajorStep( float new_major_step );

		/** Get Adjustment page size
		 * @return Adjustment page size
		 */
		float GetPageSize() const;

		/** Set Adjustment page size
		 * @param new_page_size new page size
		 */
		void SetPageSize( float new_page_size );

		/** Set all Adjustment properties
		 * @param new_value Current value.
		 * @param new_lower Minimum value.
		 * @param new_upper Maximum value.
		 * @param new_minor_step Minor change value (such as clicking on arrow button).
		 * @param new_major_step Major change value (such as clicking on the scroll area).
		 * @param new_page_size Page size (how many entries are visible / slider size).
		 */
		void Configure( float new_value, float new_lower, float new_upper, float new_minor_step, float new_major_step, float new_page_size );

		/** Increment current value
		 */
		void Increment();

		/** Decrement current value
		 */
		void Decrement();

		/** Increment current value by a page
		 */
		void IncrementPage();

		/** Decrement current value by a page
		 */
		void DecrementPage();

		// Signals.
		static Signal::SignalID OnChange; //!< Fired when parameter changed.

	private:
		/** Ctor.
		 * @param value Current value.
		 * @param lower Minimum value.
		 * @param upper Maximum value.
		 * @param minor_step Minor change value (such as clicking on arrow button).
		 * @param major_step Major change value (such as clicking on the scroll area).
		 * @param page_size Page size (how many entries are visible / slider size).
		 */
		Adjustment( float value, float lower, float upper, float minor_step, float major_step, float page_size );

		float m_value;
		float m_lower;
		float m_upper;
		float m_minor_step;
		float m_major_step;
		float m_page_size;
};

}
