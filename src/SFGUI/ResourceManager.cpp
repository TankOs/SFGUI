#include <SFGUI/ResourceManager.hpp>
#include <SFGUI/FileResourceLoader.hpp>

#ifdef SFGUI_INCLUDE_FONT
#include <SFGUI/DejaVuSansFont.hpp>
#endif

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
	{
		FontMap::const_iterator font_iter( m_fonts.find( path ) );

		if( font_iter != m_fonts.end() ) {
			return font_iter->second.first;
		}
	}

	if( path == "Default" ) {
		if( m_use_default_font ) {
#ifdef SFGUI_INCLUDE_FONT
			const sf::Font* font = new sf::Font( LoadDejaVuSansFont() );
#else
#ifdef SFGUI_DEBUG
			std::cerr << "SFGUI warning: No default font available. (SFGUI_INCLUDE_FONT = FALSE)\n";
#endif
			const sf::Font* font = new sf::Font();
#endif
			m_fonts[path] = FontPair( font, true );

			return font;
		}

		return NULL;
	}

	// Try to load.
	ResourceLoader* loader( GetMatchingLoader( path ) );
	if( !loader ) {
		const sf::Font* font = NULL;

		if( m_use_default_font ) {
			FontMap::const_iterator font_iter( m_fonts.find( path ) );

			if( font_iter == m_fonts.end() ) {
#ifdef SFGUI_INCLUDE_FONT
				font = new sf::Font( LoadDejaVuSansFont() );
#else
#ifdef SFGUI_DEBUG
				std::cerr << "SFGUI warning: No default font available. (SFGUI_INCLUDE_FONT = FALSE)\n";
#endif
				font = new sf::Font();
#endif
				m_fonts[path] = FontPair( font, true );
			}
			else {
				font = font_iter->second.first;
			}

		}
		return font;
	}

	const sf::Font* font( loader->LoadFont( GetFilename( path, *loader ) ) );

	if( !font ) {
		if( m_use_default_font ) {
			FontMap::const_iterator font_iter( m_fonts.find( path ) );

			if( font_iter == m_fonts.end() ) {
#ifdef SFGUI_INCLUDE_FONT
				font = new sf::Font( LoadDejaVuSansFont() );
#else
#ifdef SFGUI_DEBUG
				std::cerr << "SFGUI warning: No default font available. (SFGUI_INCLUDE_FONT = FALSE)\n";
#endif
				font = new sf::Font();
#endif
				m_fonts[path] = FontPair( font, true );
			}
			else {
				font = font_iter->second.first;
			}
		}
		return font;
	}

	// Cache.
	m_fonts[path] = FontPair( font, true );
	return font;
}

const sf::Image* ResourceManager::GetImage( const std::string& path ) {
	ImageMap::const_iterator image_iter( m_images.find( path ) );

	if( image_iter != m_images.end() ) {
		return image_iter->second.first;
	}

	// Try to load.
	ResourceLoader* loader( GetMatchingLoader( path ) );
	if( !loader ) {
		return NULL;
	}

	const sf::Image* image( loader->LoadImage( GetFilename( path, *loader ) ) );

	if( !image ) {
		return NULL;
	}

	// Cache.
	m_images[path] = ImagePair( image, true );
	return image;
}

ResourceLoader* ResourceManager::GetMatchingLoader( const std::string& path ) {
	if( path.empty() || ( path == "Default" ) ) {
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

	ImageMap::iterator tex_iter( m_images.begin() );
	ImageMap::iterator tex_iter_end( m_images.end() );

	for( ; tex_iter != tex_iter_end; ++tex_iter ) {
		if( tex_iter->second.second ) {
			delete tex_iter->second.first;
		}
	}

	m_loaders.clear();
	m_fonts.clear();
	m_images.clear();
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

void ResourceManager::AddImage( const std::string& path, const sf::Image& image, bool managed ) {
	ImageMap::iterator image_iter( m_images.find( path ) );

	if( image_iter != m_images.end() && image_iter->second.second ) {
		delete image_iter->second.first;
	}

	m_images[path] = ImagePair( &image, managed );
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

	ImageMap::const_iterator tex_iter( other.m_images.begin() );
	ImageMap::const_iterator tex_iter_end( other.m_images.end() );

	for( ; tex_iter != tex_iter_end; ++tex_iter ) {
		if( tex_iter->second.second ) {
			const sf::Image* new_image( new sf::Image( *tex_iter->second.first ) );
			AddImage( tex_iter->first, *new_image, true );
		}
		else {
			AddImage( tex_iter->first, *tex_iter->second.first, false );
		}
	}
}

void ResourceManager::SetDefaultFont( const sf::Font& font ) {
	AddFont( "", font, false );
}

}
