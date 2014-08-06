#include <SFGUI/Signal.hpp>

namespace {

unsigned int serial = 1;
sfg::Signal::SignalID last_guid = 0;

}

namespace sfg {

Signal::Signal( Signal&& other ) :
	m_delegates( std::move( other.m_delegates ) )
{
}

Signal& Signal::operator=( Signal&& other ) {
	m_delegates = std::move( other.m_delegates );
	return *this;
}

unsigned int Signal::Connect( std::function<void()> delegate ) {
	if( !m_delegates ) {
		m_delegates.reset( new DelegateMap );
	}

	(*m_delegates)[serial] = delegate;
	return serial++;
}

void Signal::operator()() const {
	if( !m_delegates ) {
		return;
	}

	for( const auto& delegate : *m_delegates ) {
		delegate.second();
	}
}

void Signal::Disconnect( unsigned int serial ) {
	if( !m_delegates ) {
		return;
	}

	m_delegates->erase( serial );

	if( m_delegates->empty() ) {
		m_delegates.reset();
	}
}

Signal::SignalID Signal::GetGUID() {
	return ++last_guid;
}

Signal& SignalContainer::operator[]( const Signal::SignalID& id ) {
	if( !m_signals ) {
		m_signals.reset( new SignalMap );
	}

	// Find signal in the map.
	auto signal_iter = m_signals->find( id );

	if( signal_iter == m_signals->end() ) {
		// Requested signal is not present in map.
		// Insert a new signal and set the iterator to point to it.
		signal_iter = m_signals->insert(
			std::pair<Signal::SignalID, Signal>( id, Signal() )
		).first;
	}

	// Return the signal.
	return signal_iter->second;
}

void SignalContainer::Emit( const Signal::SignalID& id ) {
	if( !m_signals || !id ) {
		return;
	}

	auto signal_iter = m_signals->find( id );

	if( signal_iter != m_signals->end() ) {
		signal_iter->second();
	}
}

}
