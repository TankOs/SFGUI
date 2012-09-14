#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/ResourceLoader.hpp>

namespace sfg {

/** Resource loader for loading from plain files.
 * This is also the fallback loader used by engines.
 */
class SFGUI_API FileResourceLoader : public ResourceLoader {
	public:
		/** Dtor.
		 */
		~FileResourceLoader();

		const sf::Font* LoadFont( const std::string& path );
		const sf::Image* LoadImage( const std::string& path );
		const std::string& GetIdentifier() const;
};

}
