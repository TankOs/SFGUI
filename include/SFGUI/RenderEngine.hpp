#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Widget.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/String.hpp>
#include <boost/any.hpp>
#include <string>
#include <map>

namespace sfg {

class Window;
class Button;
class Label;

/** Abstract base class for widget rendering.
 */
class SFGUI_API RenderEngine {
	public:
		/** Dtor.
		 */
		virtual ~RenderEngine();

		/** Create drawable for window widgets.
		 * @param window Widget.
		 * @param target Render target the drawable is created for.
		 * @return New drawable object (unmanaged memory!).
		 */
		virtual sf::Drawable* CreateWindowDrawable( boost::shared_ptr<Window> window, const sf::RenderTarget& target ) const = 0;

		/** Create drawable for button widgets.
		 * @param button Widget.
		 * @param target Render target the drawable is created for.
		 * @return New drawable object (unmanaged memory!).
		 */
		virtual sf::Drawable* CreateButtonDrawable( boost::shared_ptr<Button> button, const sf::RenderTarget& target ) const = 0;

		/** Create drawable for label widgets.
		 * @param label Widget.
		 * @param target Render target the drawable is created for.
		 * @return New drawable object (unmanaged memory!).
		 */
		virtual sf::Drawable* CreateLabelDrawable( boost::shared_ptr<Label> label, const sf::RenderTarget& target ) const = 0;

		/** Get metrics of a text string.
		 * @param string String.
		 * @param font Font.
		 * @param font_size Font size.
		 * @return Metrics.
		 */
		sf::Vector2f GetTextMetrics( const sf::String& string, const sf::Font& font, unsigned int font_size );

		/** Set property.
		 * @param property Name of property.
		 * @param value Value.
		 */
		template <typename T>
		void SetProperty( const std::string& property, const T& value );

		/** Get property.
		 * @param property Name of property.
		 * @param default_ Default value.
		 * @return Value or default_ in case property doesn't exist.
		 */
		template <typename T>
		const T GetProperty( const std::string& property, const T& default_ ) const;

		/** Load a fron from file.
		 * If the proper file was loaded before, it gets returned immediately.
		 * @param filename Filename.
		 * @return Font or sf::Font::GetDefaultFont() if failed to load.
		 */
		const sf::Font& LoadFontFromFile( const std::string& filename ) const;

		/** Utility method to get property of widget or, if it doesn't exist, of render engine.
		 * @param widget Widget.
		 * @param property Property name.
		 * @param default_ Default value.
		 * @return Widget's property value or render engine's property value or default_.
		 */
		template <typename T>
		const T GetWidgetProperty( boost::shared_ptr<const Widget> widget, const std::string& property, const T& default_ ) const;

	private:
		typedef std::map<const std::string, boost::any>  PropertiesMap;
		typedef std::map<const std::string, sf::Font>  FontsMap;

		PropertiesMap  m_props;
		mutable FontsMap  m_fonts;
};

}

#include "RenderEngine.inl"
