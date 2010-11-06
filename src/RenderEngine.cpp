#include <SFGUI/RenderEngine.hpp>
#include <SFML/Graphics/Text.hpp>
#include <sstream>

namespace sfg {

RenderEngine::~RenderEngine() {
}

// TODO: Font and size.
sf::Vector2f RenderEngine::GetMetrics( const sf::String& string ) const {
	sf::Text  text( string, sf::Font::GetDefaultFont(), 16.f );

	return sf::Vector2f( text.GetRect().Width, text.GetRect().Height );
}

}
