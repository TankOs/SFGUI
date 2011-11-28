namespace sfg {

template <class Class>
unsigned int Signal::Connect( void(Class::*function)(), Class* object ) {
	m_delegates[m_serial] = Delegate( object, function );
	return m_serial++;
}

}
