#include <SFGUI/ThemeLoader.hpp>

namespace sfg {

ThemeLoader::Observer::Observer( Engine& engine_ ) :
	engine( engine_ )
{
}

void ThemeLoader::Observer::operator()( const std::string& key, const std::string& value ) {
	Engine::PropertyType type( engine.GetPropertyType( key ) );

	if( type == Engine::Undefined ) {
		return;
	}

	if( type == Engine::UnsignedInteger ) {
		std::stringstream sstr( value );
		unsigned int integer( 0 );

		sstr >> integer;
		if( !sstr ) {
			std::cerr << "Invalid integer value for property " << key << "." << std::endl;
		}
		else {
			engine.SetProperty<unsigned int>( key, integer );
			std::cout << "Set " << key << " to int " << integer << std::endl;
		}
	}
}

}
