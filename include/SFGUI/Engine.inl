#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <iomanip>

namespace sfg {

template <typename T>
T Engine::GetProperty( const std::string& property, boost::shared_ptr<const Widget> widget ) const {
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
					try {
						return boost::lexical_cast<T>( pair.second );
					}
					catch( const boost::bad_lexical_cast& e ) {
						std::string error_message( "GetProperty: Unable to convert string to requested type. " );
						error_message += "Path: " + pair.first->BuildString();
						error_message += " Property: " + property;
						error_message += " Requested type: ";
						error_message += typeid( T ).name();
						error_message += " Value: " + pair.second;
						throw BadValue( error_message );
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
					try {
						return boost::lexical_cast<T>( pair.second );
					}
					catch( const boost::bad_lexical_cast& e ) {
						std::string error_message( "GetProperty: Unable to convert string to requested type. " );
						error_message += "Path: " + pair.first->BuildString();
						error_message += " Property: " + property;
						error_message += " Requested type: ";
						error_message += typeid( T ).name();
						error_message += " Value: " + pair.second;
						throw BadValue( error_message );
					}
				}
			}
		}
	}

	// No match/errors, return default.
	return default_;
}

template <typename T>
bool Engine::SetProperty( const std::string& selector, const std::string& property, const T& value ) {
	sfg::Selector::Ptr selector_object( sfg::Selector::Create( selector ) );

	if( !selector_object ) {
		// Invalid selector string given.
		return false;
	}

	// Convert value into string.
	std::string str_value;

	try {
		str_value = boost::lexical_cast<std::string>( value );
	}
	catch( const boost::bad_lexical_cast& e ) {
		return false;
	}

	// If the selector does already exist, we'll remove it to make sure the newly
	// added value will get a higher priority than the previous one, because
	// that's the expected behaviour (LIFO).
	SelectorValueList& list( m_properties[property][selector_object->GetWidgetName()] ); // Shortcut.
	SelectorValueList::iterator list_begin( list.begin() );
	SelectorValueList::iterator list_end( list.end() );

	for( ; list_begin != list_end; ++list_begin ) {
		if( *list_begin->first == *selector_object ) {
			// Equal, remove.
			list.erase( list_begin );
			break;
		}
	}

	// Insert at top to get highest priority.
	list.push_front( SelectorValuePair( selector_object, str_value ) );

	return true;
}

}
