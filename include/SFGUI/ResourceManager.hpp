#pragma once

#include <SFGUI/Config.hpp>

#include <map>
#include <string>

namespace sf {
class Font;
class Texture;
}

namespace sfg {

class ResourceLoader;

/** Resource manager.
 * The manager uses a ResourceLoader to load resources that are requested and
 * haven't been loaded before. It's possible to add any number of different
 * resource managers to support multiple sources (file, ZIP, whatever). Paths
 * must have the correct loader prefix. In case of loading from files, a prefix
 * is not needed. E.g. for loading from ZIP files, a loader may have the prefix
 * "zip", so paths must begin with "zip:".
 */
class SFGUI_API ResourceManager {
	public:
		/** Ctor.
		 * @param use_default_font Use SFML's default font if custom font fails to load.
		 */
		ResourceManager( bool use_default_font = true );

		/** Dtor.
		 */
		~ResourceManager();

		/** Clear manager, i.e. destroy all resources and loaders.
		 */
		void Clear();

		/** Create a loader.
		 * @return Created loader (if a loader with the same identifier already exists, it's returned).
		 */
		template <class Loader>
		ResourceLoader& CreateLoader();

		/** Get a loader.
		 * @param id Identifier.
		 * @return Loader or NULL if not found.
		 */
		ResourceLoader* GetLoader( const std::string& id );

		/** Get font.
		 * Will be loaded if not done so before.
		 * @param path Path.
		 * @return Font or NULL if failed to load.
		 */
		const sf::Font* GetFont( const std::string& path );

		/** Get texture.
		 * Will be loaded if not done so  before.
		 * @param path Path.
		 * @return Texture or NULL if failed to load.
		 */
		const sf::Texture* GetTexture( const std::string& path );

	private:
		typedef std::map<const std::string, ResourceLoader*> LoaderMap;
		typedef std::map<const std::string, const sf::Font*> FontMap;
		typedef std::map<const std::string, const sf::Texture*> TextureMap;

		ResourceLoader* GetMatchingLoader( const std::string& path );
		std::string GetFilename( const std::string& path, const ResourceLoader& loader );

		LoaderMap m_loaders;
		FontMap m_fonts;
		TextureMap m_textures;

		bool m_use_default_font;
};

}

#include "ResourceManager.inl"
