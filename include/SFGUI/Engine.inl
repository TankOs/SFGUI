#include <sstream>
#include <iomanip>

namespace sfg {

template <typename T>
T Engine::GetProperty( const std::string& property, std::shared_ptr<const Widget> widget ) const {
	static const T default_ = T();

	const std::string* value( GetValue( property, widget ) );
	if( !value ) {
		return default_;
	}

	// If string is requested, just return the value.
	if( typeid( T ) == typeid( std::string ) ) {
		// reinterpret_cast is needed here to fool the compiler. Else it will try
		// to compile "return *value" for all Ts, which may fail for
		// GetProperty<int> here, because it would be tried to return a std::string
		// in this case which fails.
		return *reinterpret_cast<const T*>( value );
	}

	// Convert value.
	T out_value;
	std::stringstream sstr( *value );
	sstr >> out_value;

	if( sstr.fail() ) {
		std::string error_message( "GetProperty: Unable to convert string to requested type." );
		error_message += " Property: " + property;
		error_message += " Requested type: ";
		error_message += typeid( T ).name();
		error_message += " Value: " + *value;
		throw BadValue( error_message );
	}

	return out_value;
}

template <typename T>
bool Engine::SetProperty( const std::string& selector, const std::string& property, const T& value ) {
	sfg::Selector::Ptr selector_object( sfg::Selector::Create( selector ) );

	if( !selector_object ) {
		// Invalid selector string given.
		return false;
	}

	// Convert value into string.
	std::stringstream sstr;
	
	sstr << value;

	if( !sstr ) {
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
	list.push_front( SelectorValuePair( selector_object, sstr.str() ) );

	return true;
}

}
