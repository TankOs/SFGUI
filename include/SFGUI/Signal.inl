namespace sfg {

template <class Class>
unsigned int Signal::Connect( void(Class::*function)(), Class* object ) {
	if( !m_delegates ) {
		m_delegates = new DelegateMap;
	}

	(*m_delegates)[m_serial] = Delegate( object, function );
	return m_serial++;
}

}
