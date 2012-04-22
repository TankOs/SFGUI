#include <SFGUI/Object.hpp>

namespace sfg {

Object::Object() {
}

Object::~Object() {
}

Signal& Object::GetSignal( SignalType type ) {
	return m_signals[type];
}

SignalContainer& Object::GetSignals() {
	return m_signals;
}

}
