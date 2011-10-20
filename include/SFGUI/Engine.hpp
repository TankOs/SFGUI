#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Widget.hpp>
#include <SFGUI/Selector.hpp>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/String.hpp>
#include <string>
#include <map>
#include <stdexcept>

namespace sf {
std::ostream& operator<<( std::ostream& stream, const Color& color );
std::istream& operator>>( std::istream& stream, Color& color );
}

namespace sfg {

class Window;
class Button;
class Label;
class Entry;
class Scale;
class Scrollbar;
class ScrolledWindow;

/** Abstract base class for widget rendering.
 */
class SFGUI_API Engine {
	public:
		typedef std::runtime_error BadValue; //<! Thrown when value can't be converted to or from string.

		/** Dtor.
		 */
		virtual ~Engine();

		/** Create drawable for window widgets.
		 * @param window Widget.
		 * @return New drawable object (unmanaged memory!).
		 */
		virtual sf::Drawable* CreateWindowDrawable( boost::shared_ptr<Window> window ) const = 0;

		/** Create drawable for button widgets.
		 * @param button Widget.
		 * @return New drawable object (unmanaged memory!).
		 */
		virtual sf::Drawable* CreateButtonDrawable( boost::shared_ptr<Button> button ) const = 0;

		/** Create drawable for label widgets.
		 * @param label Widget.
		 * @return New drawable object (unmanaged memory!).
		 */
		virtual sf::Drawable* CreateLabelDrawable( boost::shared_ptr<Label> label ) const = 0;

		/** Create drawable for entry widgets.
		 * @param entry Widget.
		 * @return New drawable object (unmanaged memory!).
		 */
		virtual sf::Drawable* CreateEntryDrawable( boost::shared_ptr<Entry> entry ) const = 0;

		/** Create drawable for scale widgets.
		 * @param scale Widget.
		 * @return New drawable object (unmanaged memory!).
		 */
		virtual sf::Drawable* CreateScaleDrawable( boost::shared_ptr<Scale> scale ) const = 0;

		/** Create drawable for scrollbar widgets.
		 * @param scrollbar Widget.
		 * @return New drawable object (unmanaged memory!).
		 */
		virtual sf::Drawable* CreateScrollbarDrawable( boost::shared_ptr<Scrollbar> scrollbar ) const = 0;

		/** Create drawable for scrolled window widgets.
		 * @param scrolled_window Widget.
		 * @return New drawable object (unmanaged memory!).
		 */
		virtual sf::Drawable* CreateScrolledWindowDrawable( boost::shared_ptr<ScrolledWindow> scrolled_window ) const = 0;

		/** Get line height of a font.
		 * @param font Font.
		 * @param font_size Font size.
		 * @return Line height.
		 */
		float GetLineHeight( const sf::Font& font, unsigned int font_size ) const;

		/** Get metrics of a text string.
		 * @param string String.
		 * @param font Font.
		 * @param font_size Font size.
		 * @return Metrics.
		 */
		sf::Vector2f GetTextMetrics( const sf::String& string, const sf::Font& font, unsigned int font_size ) const;

		/** Set property.
		 * @param selector selector string (leave empty for all widgets).
		 * @param property Property.
		 * @param value Value.
		 * @return true on success, false when: Invalid selector or invalid property.
		 * @throws BadValue when value couldn't be converted to string.
		 */
		template <typename T>
		bool SetProperty( const std::string& selector, const std::string& property, const T& value );

		/** Get property.
		 * When widget is specified, the property will be searched there, at first.
		 * @param property Name of property.
		 * @param widget Widget to be searched for property, first (optional).
		 * @return Value or T() in case property doesn't exist.
		 */
		template <typename T>
		T GetProperty( const std::string& property, boost::shared_ptr<const Widget> widget = Widget::Ptr() ) const;

		/** Load a font from file.
		 * If the proper file was loaded before, it gets returned immediately.
		 * @param filename Filename.
		 * @return Font or sf::Font::GetDefaultFont() if failed to load.
		 */
		const sf::Font& LoadFontFromFile( const std::string& filename ) const;

	private:
		typedef std::map<const std::string, sf::Font> FontMap;

		typedef std::pair<Selector::PtrConst, std::string> SelectorValuePair;
		typedef std::list<SelectorValuePair> SelectorValueList;
		typedef std::map<const std::string, SelectorValueList> WidgetNameMap;
		typedef std::map<const std::string, WidgetNameMap> PropertyMap;

		PropertyMap m_properties;

		mutable FontMap m_fonts;
};

}

#include "Engine.inl"
