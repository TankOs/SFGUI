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
class Entry;
class Scale;
class Scrollbar;

/** Abstract base class for widget rendering.
 */
class SFGUI_API Engine {
	public:
		/** Property type.
		 */
		enum PropertyType {
			Integer = 0,
			UnsignedInteger,
			Float,
			String,
			Color,
			Undefined = -1
		};

		/** Dtor.
		 */
		virtual ~Engine();

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

		/** Create drawable for entry widgets.
		 * @param entry Widget.
		 * @param target Render target the drawable is created for.
		 * @return New drawable object (unmanaged memory!).
		 */
		virtual sf::Drawable* CreateEntryDrawable( boost::shared_ptr<Entry> entry, const sf::RenderTarget& target ) const = 0;

		/** Create drawable for scale widgets.
		 * @param scale Widget.
		 * @param target Render target the drawable is created for.
		 * @return New drawable object (unmanaged memory!).
		 */
		virtual sf::Drawable* CreateScaleDrawable( boost::shared_ptr<Scale> scale, const sf::RenderTarget& target ) const = 0;

		/** Create drawable for scrollbar widgets.
		 * @param scrollbar Widget.
		 * @param target Render target the drawable is created for.
		 * @return New drawable object (unmanaged memory!).
		 */
		virtual sf::Drawable* CreateScrollbarDrawable( boost::shared_ptr<Scrollbar> scrollbar, const sf::RenderTarget& target ) const = 0;

		/** Get metrics of a text string.
		 * @param string String.
		 * @param font Font.
		 * @param font_size Font size.
		 * @return Metrics.
		 */
		sf::Vector2f GetTextMetrics( const sf::String& string, const sf::Font& font, unsigned int font_size ) const;

		/** Set property.
		 * @param property Name of property.
		 * @param value Value.
		 */
		template <typename T>
		void SetProperty( const std::string& property, const T& value );

		/** Get property.
		 * When widget is specified, the property will be searched there, at first.
		 * @param property Name of property.
		 * @param widget Widget to be searched for property, first (optional).
		 * @return Value or T() in case property doesn't exist.
		 */
		template <typename T>
		const T& GetProperty( const std::string& property, boost::shared_ptr<const Widget> widget = Widget::Ptr() ) const;

		/** Load a fron from file.
		 * If the proper file was loaded before, it gets returned immediately.
		 * @param filename Filename.
		 * @return Font or sf::Font::GetDefaultFont() if failed to load.
		 */
		const sf::Font& LoadFontFromFile( const std::string& filename ) const;

		/** Get type of registered property.
		 * @param name Property name.
		 * @return Type or Undefined if not set.
		 */
		PropertyType GetPropertyType( const std::string& name ) const;

	protected:
		/** Register property string and type.
		 * This is mainly used by theme loaders to determine what properties exist
		 * with what types.
		 * @param name Property name.
		 * @param type Type.
		 */
		void RegisterProperty( const std::string& name, PropertyType type );

	private:
		typedef std::map<const std::string, boost::any> PropertiesMap;
		typedef std::map<const std::string, PropertyType> PropertyTypesMap;
		typedef std::map<const std::string, sf::Font> FontsMap;

		PropertiesMap m_props;
		PropertyTypesMap m_prop_types;
		mutable FontsMap m_fonts;
};

}

#include "Engine.inl"
