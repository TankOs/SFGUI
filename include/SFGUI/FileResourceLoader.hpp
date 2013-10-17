#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/ResourceLoader.hpp>
#include <memory>

namespace sfg {

/** Resource loader for loading from plain files.
 * This is also the fallback loader used by engines.
 */
class SFGUI_API FileResourceLoader : public ResourceLoader {
	public:
		virtual std::shared_ptr<const sf::Font> LoadFont( const std::string& path ) const override;
		virtual std::shared_ptr<const sf::Image> LoadImage( const std::string& path ) const override;
		const std::string& GetIdentifier() const;
};

}
