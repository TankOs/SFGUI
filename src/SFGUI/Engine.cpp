#include <SFGUI/Engine.hpp>

#include <SFML/Graphics/Text.hpp>
#include <sstream>

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

	FontsMap::const_iterator  iter( m_fonts.find( filename ) );

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

void Engine::RegisterProperty( const std::string& name, PropertyType type ) {
	m_prop_types[name] = type;
}

Engine::PropertyType Engine::GetPropertyType( const std::string& name ) const {
	PropertyTypesMap::const_iterator iter( m_prop_types.find( name ) );

	if( iter == m_prop_types.end() ) {
		return Undefined;
	}

	return iter->second;
}

}
