#include <SFGUI/Signal.hpp>

namespace sfg {

Signal::Signal() :
	m_serial( 1 ),
	m_delegates_count( 0 )
{
}

unsigned int Signal::Connect( Delegate delegate ) {
	DelegatePair delegate_pair = { m_serial, delegate };
	m_delegates.push_back( delegate_pair );
	++m_delegates_count;
	return m_serial++;
}

void Signal::operator()() const {
	for( std::size_t index = 0; index < m_delegates_count; ++index ) {
		m_delegates[index].delegate();
	}
}

}
