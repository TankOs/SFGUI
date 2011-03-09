#include <SFGUI/RenderEngine.hpp>
#include <SFML/Graphics/Text.hpp>
#include <sstream>

namespace sfg {

RenderEngine::~RenderEngine() {
}

sf::Vector2f RenderEngine::GetTextMetrics( const sf::String& string, const sf::Font& font, unsigned int font_size ) {
	sf::Text  text( string, font, font_size );
	return sf::Vector2f( text.GetRect().Width, text.GetRect().Height );
}

const sf::Font& RenderEngine::LoadFontFromFile( const std::string& filename ) const {
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


}
