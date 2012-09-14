#pragma once

#include <SFGUI/Config.hpp>

#include <map>
#include <string>

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

		/** Copy ctor.
		 * @param other Other manager.
		 */
		ResourceManager( const ResourceManager& other );

		/** Dtor.
		 */
		~ResourceManager();

		/** Assignment.
		 * @param other Other manager.
		 * @return this.
		 */
		ResourceManager& operator=( const ResourceManager& other );

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

		/** Get image.
		 * Will be loaded if not done so before.
		 * @param path Path.
		 * @return Image or NULL if failed to load.
		 */
		const sf::Image* GetImage( const std::string& path );

		/** Add font.
		 * A resource with the same path will be replaced.
		 * @param path Path (or ID or whatever).
		 * @param font Font to add.
		 * @param managed true to manage destruction of resource.
		 */
		void AddFont( const std::string& path, const sf::Font& font, bool managed );

		/** Add image.
		 * A resource with the same path will be replaced.
		 * @param path Path (or ID or whatever).
		 * @param image Image to add.
		 * @param managed true to manage destruction of resource.
		 */
		void AddImage( const std::string& path, const sf::Image& image, bool managed );

		/** Sets the default font that SFGUI will use if
		 * you do not specify what font to use for a widget.
		 * @param font Your default sf::Font. It must stay valid until SFGUI is done with it.
		 */
		void SetDefaultFont( const sf::Font& font );

	private:
		typedef std::map<const std::string, ResourceLoader*> LoaderMap;

		typedef std::pair<const sf::Font*, bool> FontPair;
		typedef std::pair<const sf::Image*, bool> ImagePair;

		typedef std::map<const std::string, FontPair> FontMap;
		typedef std::map<const std::string, ImagePair> ImageMap;

		void CopyFrom( const ResourceManager& other );
		ResourceLoader* GetMatchingLoader( const std::string& path );
		std::string GetFilename( const std::string& path, const ResourceLoader& loader );

		LoaderMap m_loaders;
		FontMap m_fonts;
		ImageMap m_images;

		bool m_use_default_font;
};

}

#include "ResourceManager.inl"
