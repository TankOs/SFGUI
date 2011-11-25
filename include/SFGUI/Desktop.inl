namespace sfg {

template <class T>
void Desktop::UseEngine() {
	delete m_engine;
	m_engine = new T;
	m_context.SetEngine( *m_engine );
	Refresh();
}

template <typename T>
bool Desktop::SetProperty( const std::string& selector, const std::string& property, const T& value ) {
	if( !m_context.GetEngine().SetProperty( selector, property, value ) ) {
		return false;
	}

	Refresh();
	return true;
}

template <typename T>
T Desktop::GetProperty( const std::string& property, SharedPtr<const Widget> widget ) const {
	return m_context.GetEngine().GetProperty<T>( property, widget );
}

}
