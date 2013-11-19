#include <SFGUI/FileResourceLoader.hpp>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Image.hpp>
#include <memory>

namespace sfg {

std::shared_ptr<const sf::Font> FileResourceLoader::LoadFont( const std::string& path ) const {
	auto font = std::make_shared<sf::Font>();

	if( !font->loadFromFile( path ) ) {
		return std::shared_ptr<const sf::Font>();
	}

	return font;
}

std::shared_ptr<const sf::Image> FileResourceLoader::LoadImage( const std::string& path ) const {
	auto image = std::make_shared<sf::Image>();

	if( !image->loadFromFile( path ) ) {
		return std::shared_ptr<const sf::Image>();
	}

	return image;
}

const std::string& FileResourceLoader::GetIdentifier() const {
	static const std::string id( "file" );
	return id;
}

}
