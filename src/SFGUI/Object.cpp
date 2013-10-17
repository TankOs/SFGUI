#include <SFGUI/Object.hpp>

namespace sfg {

Signal& Object::GetSignal( Signal::SignalID& id ) {
	if( !id ) {
		id = Signal::GetGUID();
	}

	return m_signals[id];
}

SignalContainer& Object::GetSignals() {
	return m_signals;
}

}
