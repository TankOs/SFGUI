#pragma once

#include <SFGUI/Config.hpp>

#include <string>
#include <memory>

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
		virtual ~ResourceLoader() = default;

		/** Load font.
		 * @param path Path.
		 * @return Font or std::shared_ptr<const sf::Font>() if failed to load.
		 */
		virtual std::shared_ptr<const sf::Font> LoadFont( const std::string& path ) const = 0;

		/** Load image.
		 * @param path Path.
		 * @return Image or std::shared_ptr<const sf::Image>() if failed to load.
		 */
		virtual std::shared_ptr<const sf::Image> LoadImage( const std::string& path ) const = 0;

		/** Get identifier.
		 * @return Identifier.
		 */
		virtual const std::string& GetIdentifier() const = 0;
};

}
