#include <SFGUI/Engines/Bob/TextureManager.hpp>
#include <SFGUI/Renderer.hpp>

#include <SFML/Graphics/Image.hpp>

namespace sfg {
namespace eng {
namespace bob {

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
}

SharedPtr< Primitive::Texture > TextureManager::GetTexture( const sf::Image *image )
{
	TextureMap::const_iterator it( m_textures.find( image ) );
	if( it != m_textures.end() ) {
		return it->second;
	}

	if(image == NULL){
		return SharedPtr< Primitive::Texture >();
	}

	SharedPtr< Primitive::Texture > texture_handle( Renderer::Get().LoadImage( *image ) );
	m_textures[image] = texture_handle;

	return texture_handle;
}

void TextureManager::UnloadTexture( const sf::Image *image )
{
	m_textures.erase( image );
}

void TextureManager::swap( TextureManager& rhs )
{
	m_textures.swap(rhs.m_textures);
}

}
}
}

void swap( sfg::eng::bob::TextureManager& rhs, sfg::eng::bob::TextureManager& lhs )
{
	rhs.swap(lhs);
}
