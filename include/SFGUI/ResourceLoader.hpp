#pragma once

#include <SFGUI/Config.hpp>

#include <string>

namespace sf {
class Font;
class Image;
}

namespace sfg {

/** Resource loader baseclass.
 * A resource loader implements loading resources from specific sources. The
 * built-in resource loader is FileResourceLoader which loads resources from
 * plain files.
 *
 * All returned objects have to be unmanaged.
 */
class SFGUI_API ResourceLoader {
	public:
		/** Dtor.
		 */
		virtual ~ResourceLoader();

		/** Load font.
		 * @param path Path.
		 * @return Font or NULL if failed to load.
		 */
		virtual const sf::Font* LoadFont( const std::string& path ) = 0;

		/** Load image.
		 * @param path Path.
		 * @return Image or NULL if failed to load.
		 */
		virtual const sf::Image* LoadImage( const std::string& path ) = 0;

		/** Get identifier.
		 * @return Identifier.
		 */
		virtual const std::string& GetIdentifier() const = 0;
};

}
