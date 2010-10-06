#include <SFGUI/Theme.hpp>
#include <sstream>

namespace sfg {

sf::Color Theme::ParseColor( const std::string& str, const sf::Color& default_ ) {
	if( str.size() != 7 || str[0] != '#' ) {
		return default_;
	}

	std::stringstream  sstr;
	int                component_color[3];

	for( unsigned int component = 0; component < 3; ++component ) {
		sstr.str( str.substr( 1 + component * 2, 2 ) );
		sstr >> std::hex >> component_color[component];
		sstr.clear();
	}

	return sf::Color(
		static_cast<sf::Uint8>( component_color[0] ),
		static_cast<sf::Uint8>( component_color[1] ),
		static_cast<sf::Uint8>( component_color[2] )
	);
}

int Theme::ParseInt( const std::string& str, int default_ ) {
	std::stringstream  sstr( str );
	int  result( default_ );

	sstr >> result;

	return result;
}

}
