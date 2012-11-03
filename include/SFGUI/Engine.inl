#include <sstream>
#include <iomanip>
#include <typeinfo>

namespace sfg {

template <typename T>
T Engine::GetProperty( const std::string& property, SharedPtr<const Widget> widget ) const {
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
		throw BadValueException( error_message );
	}

	return out_value;
}

template <typename T>
bool Engine::SetProperty( const std::string& selector, const std::string& property, const T& value ) {
	std::stringstream properties;

	properties << selector << " {\n\t" << property << ": " << value << ";\n}";

	return SetProperties( properties.str() );
}

template <typename T>
bool Engine::SetProperty( const sfg::Selector::Ptr& selector, const std::string& property, const T& value ) {
	std::stringstream sstr;
	sstr << value;

	if( sstr.fail() ) {
		return false;
	}

	return SetProperty( selector, property, sstr.str() );
}

}
