#include <SFGUI/Signal.hpp>

namespace sfg {

Signal::Signal() :
	m_serial( 1 )
{
}

unsigned int Signal::Connect( Delegate delegate ) {
	m_delegates[m_serial] = delegate;
	return m_serial++;
}

void Signal::operator()() const {
	DelegateMap::const_iterator dg_iter( m_delegates.begin() );
	DelegateMap::const_iterator dg_iter_end( m_delegates.end() );

	for( ; dg_iter != dg_iter_end; ++dg_iter ) {
		dg_iter->second();
	}
}

}
