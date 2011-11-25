namespace sfg {

template <class Class>
unsigned int Signal::Connect( void(Class::*function)(), Class* object ) {
	DelegatePair delegate_pair = { m_serial, Delegate( object, function ) };
	m_delegates.push_back( delegate_pair );
	++m_delegates_count;
	return m_serial++;
}

}
