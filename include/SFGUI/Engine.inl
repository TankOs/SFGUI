#include <boost/foreach.hpp>

namespace sfg {

template <typename T>
const T& Engine::GetProperty( const std::string& property, boost::shared_ptr<const Widget> widget ) const {
	static const T default_ = T();

	// Look for property.
	PropertyMap::const_iterator prop_iter( m_properties.find( property ) );

	if( prop_iter != m_properties.end() ) {
		// Find widget-specific properties, first.
		WidgetNameMap::const_iterator name_iter( prop_iter->second.find( widget->GetName() ) );

		if( name_iter != prop_iter->second.end() ) {
			// Check against selectors.
			BOOST_FOREACH( const SelectorValuePair& pair, name_iter->second ) {
				if( pair.first->Matches( widget ) ) {
					// Found, return value.
					const T* value( boost::any_cast<const T>( &pair.second ) );

					if( value ) {
						return *value;
					}
					else {
						return default_;
					}
				}
			}
		}

		// Look for general properties now.
		name_iter = prop_iter->second.find( "" );

		if( name_iter != prop_iter->second.end() ) {
			// Check against selectors.
			BOOST_FOREACH( const SelectorValuePair& pair, name_iter->second ) {
				if( pair.first->Matches( widget ) ) {
					// Found, return value.
					const T* value( boost::any_cast<const T>( &pair.second ) );

					if( value ) {
						return *value;
					}
					else {
						return default_;
					}
				}
			}
		}
	}

	// No match/errors, return default.
	return default_;
}

template <typename T>
void Engine::SetProperty( const std::string& /*property*/, const T& /*value*/ ) {
	//m_props[property] = value;
}

template <typename T>
bool Engine::SetProperty( const std::string& selector, const std::string& property, const T& value ) {
	return SetPropertyImpl( selector, property, static_cast<T>( value ) );
}

}
