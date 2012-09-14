#include <SFGUI/FileResourceLoader.hpp>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Image.hpp>

namespace sfg {

FileResourceLoader::~FileResourceLoader() {
}

const sf::Font* FileResourceLoader::LoadFont( const std::string& path ) {
	sf::Font* font( new sf::Font );

	if( !font->loadFromFile( path ) ) {
		delete font;
		return NULL;
	}

	return font;
}

const sf::Image* FileResourceLoader::LoadImage( const std::string& path ) {
	sf::Image *image( new sf::Image );

	if( !image->loadFromFile( path ) ) {
		delete image;
		return NULL;
	}

	return image;
}

const std::string& FileResourceLoader::GetIdentifier() const {
	static const std::string id( "file" );
	return id;
}

}
