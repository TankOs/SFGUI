#include <SFGUI/Engine.hpp>

#include <SFML/Graphics/Text.hpp>
#include <sstream>

namespace sf {
std::ostream& operator<<( std::ostream& stream, const Color& color ) {
	stream
		<< std::setfill( '0' )
		<< "#"
		<< std::hex
		<< std::setw( 2 ) << static_cast<int>( color.r )
		<< std::setw( 2 ) << static_cast<int>( color.g )
		<< std::setw( 2 ) << static_cast<int>( color.b )
		<< std::setw( 2 ) << static_cast<int>( color.a )
		<< std::dec
	;
	return stream;
}

std::istream& operator>>( std::istream& stream, Color& color ) {
	std::string buffer;
	stream >> buffer;

	// Check length and leading '#'.
	if( buffer.size() != 9 || buffer[0] != '#' ) {
		stream.setstate( std::ios::failbit );
		return stream;
	}

	std::stringstream r( buffer.substr( 1, 2 ) );
	std::stringstream g( buffer.substr( 3, 2 ) );
	std::stringstream b( buffer.substr( 5, 2 ) );
	std::stringstream a( buffer.substr( 7, 2 ) );
	int r_val( 0 );
	int g_val( 0 );
	int b_val( 0 );
	int a_val( 0 );

	r >> std::hex >> r_val;
	g >> std::hex >> g_val;
	b >> std::hex >> b_val;
	a >> std::hex >> a_val;

	color.r = static_cast<sf::Uint8>( r_val );
	color.g = static_cast<sf::Uint8>( g_val );
	color.b = static_cast<sf::Uint8>( b_val );
	color.a = static_cast<sf::Uint8>( a_val );

	return stream;
}
}


namespace sfg {

Engine::~Engine() {
}

float Engine::GetLineHeight( const sf::Font& font, unsigned int font_size ) const {
	return static_cast<float>( font.GetLineSpacing( font_size ) ) + std::floor( static_cast<float>( font_size ) / 20.f ); // Last part from experimentation
}

sf::Vector2f Engine::GetTextMetrics( const sf::String& string, const sf::Font& font, unsigned int font_size ) const {
	sf::Text  text( string, font, font_size );
	return sf::Vector2f( text.GetRect().Width, text.GetRect().Height );
}

const sf::Font& Engine::LoadFontFromFile( const std::string& filename ) const {
	if( filename.empty() ) {
		return sf::Font::GetDefaultFont();
	}

	FontMap::const_iterator  iter( m_fonts.find( filename ) );

	if( iter != m_fonts.end() ) {
		return iter->second;
	}

	sf::Font  font;
	if( !font.LoadFromFile( filename ) ) {
		return sf::Font::GetDefaultFont();
	}

	m_fonts[filename] = font;
	return m_fonts[filename];
}

void Engine::ShiftBorderColors( sf::Color& light_color, sf::Color& dark_color, int offset ) const {
	// TODO: Replace by += and -=. Currently not possible with SFML (see SFML issue #114).
	light_color.r = static_cast<sf::Uint8>( std::min( 255, static_cast<int>( light_color.r ) + offset ) );
	light_color.g = static_cast<sf::Uint8>( std::min( 255, static_cast<int>( light_color.g ) + offset ) );
	light_color.b = static_cast<sf::Uint8>( std::min( 255, static_cast<int>( light_color.b ) + offset ) );

	dark_color.r = static_cast<sf::Uint8>( std::max( 0, static_cast<int>( dark_color.r ) - offset ) );
	dark_color.g = static_cast<sf::Uint8>( std::max( 0, static_cast<int>( dark_color.g ) - offset ) );
	dark_color.b = static_cast<sf::Uint8>( std::max( 0, static_cast<int>( dark_color.b ) - offset ) );
}

}
