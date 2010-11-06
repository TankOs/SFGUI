namespace sfg {

template <typename T>
const T& RenderEngine::GetProperty( const std::string& property, const T& default_ ) const {
	PropertiesMap::const_iterator  iter( m_props.find( property ) );

	if( iter == m_props.end() ) {
		return default_;
	}

	try {
		const T&  value( boost::any_cast<const T&>( iter->second ) );
		return value;
	}
	catch( ... ) {
	}

	return default_;
}

template <typename T>
const T& RenderEngine::GetWidgetProperty( boost::shared_ptr<Widget> widget, const std::string& property, const T& default_ ) const {
	if( !widget->HasProperty( property ) ) {
		return GetProperty<T>( property, default_ );
	}

	return widget->GetProperty<T>( property, default_ );
}

template <typename T>
void RenderEngine::SetProperty( const std::string& property, const T& value ) {
	m_props[property] = value;
}


}
