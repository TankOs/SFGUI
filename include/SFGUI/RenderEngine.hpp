#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Window.hpp>
#include <SFGUI/Button.hpp>
#include <boost/any.hpp>
#include <string>
#include <map>

namespace sfg {

/** Abstract base class for widget rendering.
 */
class SFGUI_API RenderEngine {
	public:
		/** Dtor.
		 */
		virtual ~RenderEngine();

		/** Create drawable for window widgets.
		 * @param window Widget.
		 * @return New drawable object (unmanaged memory!).
		 */
		virtual sf::Drawable* CreateWindowDrawable( Window::Ptr window ) const = 0;

		/** Create drawable for button widgets.
		 * @param button Widget.
		 * @return New drawable object (unmanaged memory!).
		 */
		virtual sf::Drawable* CreateButtonDrawable( Button::Ptr button ) const = 0;

		/** Get metrics of a text string.
		 * @param string String.
		 * @return Metrics.
		 */
		sf::Vector2f GetMetrics( const sf::String& string ) const;

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
		const T& GetProperty( const std::string& property, const T& default_ ) const;

	protected:
		/** Utility method to get property of widget or, if it doesn't exist, of render engine.
		 * @param widget Widget.
		 * @param property Property name.
		 * @param default_ Default value.
		 * @return Widget's property value or render engine's property value or default_.
		 */
		template <typename T>
		const T& GetWidgetProperty( Widget::Ptr widget, const std::string& property, const T& default_ ) const;

	private:
		typedef std::map<const std::string, boost::any>  PropertiesMap;

		PropertiesMap  m_props;
};

}

#include "RenderEngine.inl"
