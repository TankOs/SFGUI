#include <SFGUI/RenderEngine.hpp>
#include <SFML/Graphics/Text.hpp>
#include <sstream>

namespace sfg {

void RenderEngine::SetProperty( const std::string& property, const std::string& value ) {
	m_props[property] = value;
}

const std::string& RenderEngine::GetProperty( const std::string& property, const std::string& default_ ) const {
	PropertiesMap::const_iterator  iter( m_props.find( property ) );

	return iter != m_props.end() ? iter->second : default_;
}

int RenderEngine::GetProperty( const std::string& property, int default_ ) const {
	std::stringstream  sstr( GetProperty( property, "" ) );
	int  value( default_ );

	sstr >> value;

	return value;
}

float RenderEngine::GetProperty( const std::string& property, float default_ ) const {
	std::stringstream  sstr( GetProperty( property, "" ) );
	float  value( default_ );

	sstr >> value;

	return value;
}

// TODO: Font and size.
sf::Vector2f RenderEngine::GetMetrics( const sf::String& string ) const {
	sf::Text  text( string );

	return sf::Vector2f( text.GetRect().Width, text.GetRect().Height );
}

}
