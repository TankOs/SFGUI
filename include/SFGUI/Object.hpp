#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Signal.hpp>
#include <SFGUI/NonCopyable.hpp>
#include <SFGUI/SharedPtr.hpp>

namespace sfg {

/** Base class for all Widget, Adjustment and Tooltip objects.
 */
class SFGUI_API Object : public NonCopyable {
	public:
		typedef SharedPtr<Object> Ptr; //!< Shared pointer.
		typedef SharedPtr<const Object> PtrConst; //!< Shared pointer.

		/** Dtor.
		 */
		virtual ~Object();

		/** Request a reference to a specific signal for this object.
		 * @param id Requested signal id.
		 * @return Reference to requested signal.
		 */
		Signal& GetSignal( Signal::SignalID& id );

	protected:
		/** Constructor.
		 */
		Object();

		/** Get the signal container.
		 * @return Reference to the signal container.
		 */
		SignalContainer& GetSignals();

	private:
		// Signals.
		SignalContainer m_signals;
};

}
