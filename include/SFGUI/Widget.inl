namespace sfg {

template <typename T>
const T& Widget::GetProperty( const std::string& name, const T& default_ ) const {
	PropertiesMap::const_iterator  iter( m_properties.find( name ) );

	if( iter == m_properties.end() ) {
		return default_;
	}

	// Try to convert.
	try {
		const T&  value( boost::any_cast<const T&>( iter->second ) );
		return value;
	}
	catch( ... ) {
	}

	return default_;
}

template <typename T>
void Widget::SetProperty( const std::string& name, const T& value ) {
	m_properties[name] = value;
	Invalidate();
}

}
