#include <SFGUI/ResourceManager.hpp>
#include <SFGUI/FileResourceLoader.hpp>

#include <SFML/Graphics/Font.hpp>

namespace sfg {

ResourceManager::ResourceManager( bool use_default_font ) :
	m_use_default_font( use_default_font )
{
	// Add file resource loader as fallback.
	CreateLoader<FileResourceLoader>();
}

ResourceManager::ResourceManager( const ResourceManager& other ) {
	CopyFrom( other );
}

ResourceManager::~ResourceManager() {
	Clear();
}

ResourceManager& ResourceManager::operator=( const ResourceManager& other ) {
	CopyFrom( other );
	return *this;
}

ResourceLoader* ResourceManager::GetLoader( const std::string& id ) {
	LoaderMap::iterator loader_iter( m_loaders.find( id ) );
	return loader_iter == m_loaders.end() ? NULL : loader_iter->second;
}

const sf::Font* ResourceManager::GetFont( const std::string& path ) {
	if( path.empty() ) {
		if( m_use_default_font ) {
			return &sf::Font::GetDefaultFont();
		}

		return NULL;
	}

	FontMap::const_iterator font_iter( m_fonts.find( path ) );

	if( font_iter != m_fonts.end() ) {
		return font_iter->second.first;
	}

	// Try to load.
	ResourceLoader* loader( GetMatchingLoader( path ) );
	if( !loader ) {
		if( m_use_default_font ) {
			return &sf::Font::GetDefaultFont();
		}

		return NULL;
	}

	const sf::Font* font( loader->LoadFont( GetFilename( path, *loader ) ) );

	if( !font ) {
		return m_use_default_font ? &sf::Font::GetDefaultFont() : NULL;
	}

	// Cache.
	m_fonts[path] = FontPair( font, true );
	return font;
}

const sf::Texture* ResourceManager::GetTexture( const std::string& path ) {
	TextureMap::const_iterator texture_iter( m_textures.find( path ) );

	if( texture_iter != m_textures.end() ) {
		return texture_iter->second.first;
	}

	// Try to load.
	ResourceLoader* loader( GetMatchingLoader( path ) );
	if( !loader ) {
		return NULL;
	}

	const sf::Texture* texture( loader->LoadTexture( GetFilename( path, *loader ) ) );

	if( !texture ) {
		return NULL;
	}

	// Cache.
	m_textures[path] = TexturePair( texture, true );
	return texture;
}

ResourceLoader* ResourceManager::GetMatchingLoader( const std::string& path ) {
	if( path.empty() ) {
		return NULL;
	}

	// Extract prefix.
	std::size_t colon_pos( path.find( ':' ) );
	ResourceLoader* loader( NULL );

	if( colon_pos != std::string::npos ) {
		std::string ident( path.substr( 0, colon_pos ) );

		LoaderMap::iterator loader_iter( m_loaders.find( ident ) );

		if( loader_iter != m_loaders.end() ) {
			loader = loader_iter->second;
		}
	}

	// Use file loader as fallback.
	if( !loader ) {
		loader = m_loaders["file"];
	}

	return loader;
}

void ResourceManager::Clear() {
	LoaderMap::iterator loader_iter( m_loaders.begin() );
	LoaderMap::iterator loader_iter_end( m_loaders.end() );
	
	for( ; loader_iter != loader_iter_end; ++loader_iter ) {
		delete loader_iter->second;
	}

	FontMap::iterator font_iter( m_fonts.begin() );
	FontMap::iterator font_iter_end( m_fonts.end() );
	
	for( ; font_iter != font_iter_end; ++font_iter ) {
		if( font_iter->second.second ) {
			delete font_iter->second.first;
		}
	}

	TextureMap::iterator tex_iter( m_textures.begin() );
	TextureMap::iterator tex_iter_end( m_textures.end() );
	
	for( ; tex_iter != tex_iter_end; ++tex_iter ) {
		if( tex_iter->second.second ) {
			delete tex_iter->second.first;
		}
	}

	m_loaders.clear();
	m_fonts.clear();
	m_textures.clear();
}

std::string ResourceManager::GetFilename( const std::string& path, const ResourceLoader& loader ) {
	std::string ident( loader.GetIdentifier() + ":" );
	std::size_t ident_pos( path.find( ident ) );

	if( ident_pos == std::string::npos || ident_pos != 0 ) {
		return path;
	}

	return path.substr( ident.size() );
}

void ResourceManager::AddFont( const std::string& path, const sf::Font& font, bool managed ) {
	FontMap::iterator font_iter( m_fonts.find( path ) );

	if( font_iter != m_fonts.end() && font_iter->second.second ) {
		delete font_iter->second.first;
	}

	m_fonts[path] = FontPair( &font, managed );
}

void ResourceManager::AddTexture( const std::string& path, const sf::Texture& texture, bool managed ) {
	TextureMap::iterator texture_iter( m_textures.find( path ) );

	if( texture_iter != m_textures.end() && texture_iter->second.second ) {
		delete texture_iter->second.first;
	}

	m_textures[path] = TexturePair( &texture, managed );
}

void ResourceManager::CopyFrom( const ResourceManager& other ) {
	Clear();

	FontMap::const_iterator font_iter( other.m_fonts.begin() );
	FontMap::const_iterator font_iter_end( other.m_fonts.end() );
	
	for( ; font_iter != font_iter_end; ++font_iter ) {
		if( font_iter->second.second ) {
			const sf::Font* new_font( new sf::Font( *font_iter->second.first ) );
			AddFont( font_iter->first, *new_font, true );
		}
		else {
			AddFont( font_iter->first, *font_iter->second.first, false );
		}
	}

	TextureMap::const_iterator tex_iter( other.m_textures.begin() );
	TextureMap::const_iterator tex_iter_end( other.m_textures.end() );
	
	for( ; tex_iter != tex_iter_end; ++tex_iter ) {
		if( tex_iter->second.second ) {
			const sf::Texture* new_texture( new sf::Texture( *tex_iter->second.first ) );
			AddTexture( tex_iter->first, *new_texture, true );
		}
		else {
			AddTexture( tex_iter->first, *tex_iter->second.first, false );
		}
	}
}

}
