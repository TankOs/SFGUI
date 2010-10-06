#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Window.hpp>
#include <string>
#include <map>

namespace sfg {

/** Abstract base class for widget rendering.
 */
class SFGUI_API RenderEngine {
	public:
		/** Create drawable for window widgets.
		 * @param window Widget.
		 * @return New drawable object (unmanaged memory!).
		 */
		virtual sf::Drawable* CreateWindowDrawable( Window::Ptr window ) const = 0;

		/** Set property.
		 * @param property Name of property.
		 * @param value Value.
		 */
		void SetProperty( const std::string& property, const std::string& value );

		/** Get property.
		 * @param property Name of property.
		 * @param default_ Returned value in case of property not found.
		 * @return Value of property or default_.
		 */
		const std::string& GetProperty( const std::string& property, const std::string& default_ = "" ) const;

	private:
		typedef std::map<const std::string, std::string>  PropertiesMap;

		PropertiesMap  m_props;
};

}
