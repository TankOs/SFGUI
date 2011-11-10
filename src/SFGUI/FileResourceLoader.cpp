#include <SFGUI/FileResourceLoader.hpp>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Image.hpp>

namespace sfg {

FileResourceLoader::~FileResourceLoader() {
}

const sf::Font* FileResourceLoader::LoadFont( const std::string& path ) {
	sf::Font* font( new sf::Font );

	if( !font->LoadFromFile( path ) ) {
		delete font;
		return NULL;
	}

	return font;
}

const sf::Texture* FileResourceLoader::LoadTexture( const std::string& path ) {
	sf::Image image;

	if( !image.LoadFromFile( path ) ) {
		return NULL;
	}

	sf::Texture* texture( new sf::Texture );
	texture->LoadFromImage( image );

	return texture;
}

const std::string& FileResourceLoader::GetIdentifier() const {
	static const std::string id( "file" );
	return id;
}

}
