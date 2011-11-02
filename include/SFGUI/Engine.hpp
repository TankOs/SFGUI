#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Widget.hpp>
#include <SFGUI/Selector.hpp>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/String.hpp>
#include <string>
#include <map>
#include <list>
#include <stdexcept>

namespace sf {
SFGUI_API std::ostream& operator<<( std::ostream& stream, const Color& color );
SFGUI_API std::istream& operator>>( std::istream& stream, Color& color );
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
		virtual sf::Drawable* CreateWindowDrawable( std::shared_ptr<Window> window ) const = 0;

		/** Create drawable for button widgets.
		 * @param button Widget.
		 * @return New drawable object (unmanaged memory!).
		 */
		virtual sf::Drawable* CreateButtonDrawable( std::shared_ptr<Button> button ) const = 0;

		/** Create drawable for label widgets.
		 * @param label Widget.
		 * @return New drawable object (unmanaged memory!).
		 */
		virtual sf::Drawable* CreateLabelDrawable( std::shared_ptr<Label> label ) const = 0;

		/** Create drawable for entry widgets.
		 * @param entry Widget.
		 * @return New drawable object (unmanaged memory!).
		 */
		virtual sf::Drawable* CreateEntryDrawable( std::shared_ptr<Entry> entry ) const = 0;

		/** Create drawable for scale widgets.
		 * @param scale Widget.
		 * @return New drawable object (unmanaged memory!).
		 */
		virtual sf::Drawable* CreateScaleDrawable( std::shared_ptr<Scale> scale ) const = 0;

		/** Create drawable for scrollbar widgets.
		 * @param scrollbar Widget.
		 * @return New drawable object (unmanaged memory!).
		 */
		virtual sf::Drawable* CreateScrollbarDrawable( std::shared_ptr<Scrollbar> scrollbar ) const = 0;

		/** Create drawable for scrolled window widgets.
		 * @param scrolled_window Widget.
		 * @return New drawable object (unmanaged memory!).
		 */
		virtual sf::Drawable* CreateScrolledWindowDrawable( std::shared_ptr<ScrolledWindow> scrolled_window ) const = 0;

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
		 * @param property Name of property.
		 * @param widget Widget to be used for building the property path.
		 * @return Value or T() in case property doesn't exist.
		 */
		template <typename T>
		T GetProperty( const std::string& property, std::shared_ptr<const Widget> widget = Widget::Ptr() ) const;

		/** Load a font from file.
		 * If the proper file was loaded before, it gets returned immediately.
		 * @param filename Filename.
		 * @return Font or sf::Font::GetDefaultFont() if failed to load.
		 */
		const sf::Font& LoadFontFromFile( const std::string& filename ) const;

		/** Shift the given border colors to make them lighter and darker.
		 * @param light_color Color of the lighter border.
		 * @param dark_color Color of the darker border.
		 * @param offset Amount to shift by.
		 */
		void ShiftBorderColors( sf::Color& light_color, sf::Color& dark_color, int offset ) const;

	private:
		typedef std::map<const std::string, sf::Font> FontMap;

		typedef std::pair<Selector::PtrConst, std::string> SelectorValuePair;
		typedef std::list<SelectorValuePair> SelectorValueList;
		typedef std::map<const std::string, SelectorValueList> WidgetNameMap;
		typedef std::map<const std::string, WidgetNameMap> PropertyMap;

		const std::string* GetValue( const std::string& property, std::shared_ptr<const Widget> widget ) const;

		PropertyMap m_properties;

		mutable FontMap m_fonts;
};

}

#include "Engine.inl"
