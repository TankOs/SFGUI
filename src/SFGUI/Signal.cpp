#include <SFGUI/Signal.hpp>

namespace sfg {

unsigned int Signal::m_serial = 1;
Signal::SignalID Signal::m_last_guid = 0;

Signal::Signal() :
	m_delegates( 0 )
{
}

Signal::~Signal() {
	delete m_delegates;
}

unsigned int Signal::Connect( Delegate delegate ) {
	if( !m_delegates ) {
		m_delegates = new DelegateMap;
	}

	(*m_delegates)[m_serial] = delegate;
	return m_serial++;
}

void Signal::operator()() const {
	if( !m_delegates ) {
		return;
	}

	DelegateMap::const_iterator dg_iter( m_delegates->begin() );
	DelegateMap::const_iterator dg_iter_end( m_delegates->end() );

	for( ; dg_iter != dg_iter_end; ++dg_iter ) {
		dg_iter->second();
	}
}

void Signal::Disconnect( unsigned int serial ) {
	if( !m_delegates ) {
		return;
	}

	m_delegates->erase( serial );

	if( m_delegates->empty() ) {
		delete m_delegates;
		m_delegates = 0;
	}
}

Signal::SignalID Signal::GetGUID() {
	return ++m_last_guid;
}

SignalContainer::SignalContainer() :
	m_signals( 0 )
{
}

SignalContainer::~SignalContainer() {
	delete m_signals;
}

Signal& SignalContainer::operator[]( const Signal::SignalID& id ) {
	if( !m_signals ) {
		m_signals = new SignalMap;
	}

	// Find signal in the map.
	SignalMap::iterator signal_iter = m_signals->find( id );

	if( signal_iter == m_signals->end() ) {
		// Requested signal is not present in map.
		// Insert a new signal and set the iterator to point to it.
		signal_iter = m_signals->insert( std::pair<Signal::SignalID, Signal>( id, Signal() ) ).first;
	}

	// Return the signal.
	return signal_iter->second;
}

void SignalContainer::Emit( const Signal::SignalID& id ) {
	if( !m_signals || !id ) {
		return;
	}

	SignalMap::iterator signal_iter = m_signals->find( id );

	if( signal_iter != m_signals->end() ) {
		signal_iter->second();
	}
}

}
