#include <SFGUI/ResourceManager.hpp>
#include <SFGUI/FileResourceLoader.hpp>

#if defined( SFGUI_INCLUDE_FONT )
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

std::shared_ptr<const ResourceLoader> ResourceManager::GetLoader( const std::string& id ) {
	auto loader_iter = m_loaders.find( id );
	return loader_iter == m_loaders.end() ? std::shared_ptr<const ResourceLoader>() : loader_iter->second;
}

std::shared_ptr<const sf::Font> ResourceManager::GetFont( const std::string& path ) {
	{
		auto font_iter = m_fonts.find( path );

		if( font_iter != m_fonts.end() ) {
			return font_iter->second;
		}
	}

	if( path == "Default" ) {
		if( m_use_default_font ) {
#if defined( SFGUI_INCLUDE_FONT )
			auto font = std::make_shared<sf::Font>( LoadDejaVuSansFont() );
#else
#if defined( SFGUI_DEBUG )
			std::cerr << "SFGUI warning: No default font available. (SFGUI_INCLUDE_FONT = FALSE)\n";
#endif
			auto font = std::make_shared<sf::Font>();
#endif
			m_fonts[path] = font;

			return font;
		}

		return std::shared_ptr<const sf::Font>();
	}

	// Try to load.
	auto loader = GetMatchingLoader( path );

	if( !loader ) {
		std::shared_ptr<const sf::Font> font;

		if( m_use_default_font ) {
			auto font_iter = m_fonts.find( path );

			if( font_iter == m_fonts.end() ) {
#if defined( SFGUI_INCLUDE_FONT )
				font = std::make_shared<sf::Font>( LoadDejaVuSansFont() );
#else
#if defined( SFGUI_DEBUG )
				std::cerr << "SFGUI warning: No default font available. (SFGUI_INCLUDE_FONT = FALSE)\n";
#endif
				font = std::make_shared<sf::Font>();
#endif
				m_fonts[path] = font;
			}
			else {
				font = font_iter->second;
			}

		}
		return font;
	}

	auto font = loader->LoadFont( GetFilename( path, *loader ) );

	if( !font ) {
		if( m_use_default_font ) {
			auto font_iter = m_fonts.find( path );

			if( font_iter == m_fonts.end() ) {
#if defined( SFGUI_INCLUDE_FONT )
				font = std::make_shared<sf::Font>( LoadDejaVuSansFont() );
#else
#if defined( SFGUI_DEBUG )
				std::cerr << "SFGUI warning: No default font available. (SFGUI_INCLUDE_FONT = FALSE)\n";
#endif
				font = std::make_shared<sf::Font>();
#endif
				m_fonts[path] = font;
			}
			else {
				font = font_iter->second;
			}
		}
		return font;
	}

	// Cache.
	m_fonts[path] = font;
	return font;
}

std::shared_ptr<const sf::Image> ResourceManager::GetImage( const std::string& path ) {
	auto image_iter = m_images.find( path );

	if( image_iter != m_images.end() ) {
		return image_iter->second;
	}

	// Try to load.
	auto loader = GetMatchingLoader( path );

	if( !loader ) {
		return std::shared_ptr<const sf::Image>();
	}

	auto image = loader->LoadImage( GetFilename( path, *loader ) );

	if( !image ) {
		return std::shared_ptr<const sf::Image>();
	}

	// Cache.
	m_images[path] = image;
	return image;
}

std::shared_ptr<const ResourceLoader> ResourceManager::GetMatchingLoader( const std::string& path ) {
	if( path.empty() || ( path == "Default" ) ) {
		return std::shared_ptr<const ResourceLoader>();
	}

	// Extract prefix.
	auto colon_pos = path.find( ':' );
	std::shared_ptr<const ResourceLoader> loader;

	if( colon_pos != std::string::npos ) {
		auto ident = path.substr( 0, colon_pos );

		auto loader_iter = m_loaders.find( ident );

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
	m_loaders.clear();
	m_fonts.clear();
	m_images.clear();
}

std::string ResourceManager::GetFilename( const std::string& path, const ResourceLoader& loader ) {
	auto ident = loader.GetIdentifier() + ":";
	auto ident_pos = path.find( ident );

	if( ident_pos == std::string::npos || ident_pos != 0 ) {
		return path;
	}

	return path.substr( ident.size() );
}

void ResourceManager::AddFont( const std::string& path, std::shared_ptr<const sf::Font> font ) {
	m_fonts[path] = font;
}

void ResourceManager::AddImage( const std::string& path, std::shared_ptr<const sf::Image> image ) {
	m_images[path] = image;
}

void ResourceManager::SetDefaultFont( std::shared_ptr<const sf::Font> font ) {
	AddFont( "", font );
}

}
