#pragma once

#include <SFGUI/Config.hpp>

#include <map>
#include <string>
#include <memory>

namespace sf {
class Font;
class Image;
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

		/** Clear manager, i.e. destroy all resources and loaders.
		 */
		void Clear();

		/** Create a loader.
		 * @return Created loader (if a loader with the same identifier already exists, it's returned).
		 */
		template <class Loader>
		std::shared_ptr<const ResourceLoader> CreateLoader();

		/** Get a loader.
		 * @param id Identifier.
		 * @return Loader or std::shared_ptr<const ResourceLoader>() if not found.
		 */
		std::shared_ptr<const ResourceLoader> GetLoader( const std::string& id );

		/** Get font.
		 * Will be loaded if not done so before.
		 * @param path Path.
		 * @return Font or std::shared_ptr<const sf::Font>() if failed to load.
		 */
		std::shared_ptr<const sf::Font> GetFont( const std::string& path );

		/** Get image.
		 * Will be loaded if not done so before.
		 * @param path Path.
		 * @return Image or std::shared_ptr<const sf::Image>() if failed to load.
		 */
		std::shared_ptr<const sf::Image> GetImage( const std::string& path );

		/** Add font.
		 * A resource with the same path will be replaced.
		 * @param path Path (or ID or whatever).
		 * @param font Font to add.
		 */
		void AddFont( const std::string& path, std::shared_ptr<const sf::Font> font );

		/** Add image.
		 * A resource with the same path will be replaced.
		 * @param path Path (or ID or whatever).
		 * @param image Image to add.
		 */
		void AddImage( const std::string& path, std::shared_ptr<const sf::Image> image );

		/** Sets the default font that SFGUI will use if
		 * you do not specify what font to use for a widget.
		 * @param font Your default sf::Font. It must stay valid until SFGUI is done with it.
		 */
		void SetDefaultFont( std::shared_ptr<const sf::Font> font );

	private:
		typedef std::map<const std::string, std::shared_ptr<const ResourceLoader>> LoaderMap;
		typedef std::map<const std::string, std::shared_ptr<const sf::Font>> FontMap;
		typedef std::map<const std::string, std::shared_ptr<const sf::Image>> ImageMap;

		std::shared_ptr<const ResourceLoader> GetMatchingLoader( const std::string& path );
		std::string GetFilename( const std::string& path, const ResourceLoader& loader );

		LoaderMap m_loaders;
		FontMap m_fonts;
		ImageMap m_images;

		bool m_use_default_font;
};

}

#include "ResourceManager.inl"
