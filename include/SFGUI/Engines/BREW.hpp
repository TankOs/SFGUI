#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/RenderEngine.hpp>
#include <SFGUI/RenderQueue.hpp>
#include <SFML/Graphics/Font.hpp>
#include <map>

namespace sfg {
namespace eng {

/** BREW -- Basic Rendering Engine for Widget (reference implementation).
 */
class BREW : public RenderEngine {
	public:
		/** Ctor.
		 */
		BREW();

		sf::Drawable* CreateWindowDrawable( boost::shared_ptr<Window> window, const sf::RenderTarget& target ) const;
		sf::Drawable* CreateButtonDrawable( boost::shared_ptr<Button> button, const sf::RenderTarget& target ) const;
		sf::Drawable* CreateLabelDrawable( boost::shared_ptr<Label> label, const sf::RenderTarget& target ) const;

		/** Utility function to create borders.
		 * @param rect Rectangle.
		 * @param border_width Border width.
		 * @param light_color Color of lightened edges.
		 * @param dark_color Color of darkened edges.
		 * @return RenderQueue containing border (unmanaged memory!).
		 */
		static RenderQueue* CreateBorder( const sf::FloatRect& rect, float border_width, const sf::Color& light_color, const sf::Color& dark_color );

		/** Load a fron from file.
		 * If the proper file was loaded before, it gets returned immediately.
		 * @param filename Filename.
		 * @return Font or sf::Font::GetDefaultFont() if failed to load.
		 */
		const sf::Font& LoadFontFromFile( const std::string& filename ) const;

	private:
		typedef std::map<const std::string, sf::Font>  FontsMap;

		mutable FontsMap  m_fonts;
};

}
}
