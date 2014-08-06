#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>

#include <memory>
#include <string>
#include <deque>

namespace sf {
class Event;
class Window;
}

namespace sfg {

class Widget;

/** The desktop manages stackable children and manages an own rendering engine.
 * The desktop should be use as a managing class for all your windows/widgets.
 * Set your properties here so that your children refresh automagically.
 */
class SFGUI_API Desktop {
	public:
		/** Use a custom engine.
		 */
		template <class T>
		void UseEngine();

		/** Set property.
		 * @param selector selector string (leave empty for all widgets).
		 * @param property Property.
		 * @param value Value.
		 * @return true on success, false when: Invalid selector or invalid property.
		 * @throws BadValue when value couldn't be converted to string.
		 */
		template <typename T>
		bool SetProperty( const std::string& selector, const std::string& property, const T& value );

		/** Set multiple properties at once.
		 * @param properties CSS-like rule declarations.
		 * @return true on success, false when: rule could not be parsed.
		 */
		bool SetProperties( const std::string& properties );

		/** Get property.
		 * @param property Name of property.
		 * @param widget Widget to be used for building the property path.
		 * @return Value or T() in case property doesn't exist.
		 */
		template <typename T>
		T GetProperty( const std::string& property, std::shared_ptr<const Widget> widget = std::shared_ptr<const Widget>() ) const;

		/** Update
		 * @param seconds Elapsed time in seconds.
		 */
		void Update( float seconds );

		/** Handle event.
		 * @param event SFML event.
		 */
		void HandleEvent( const sf::Event& event );

		/** Add widget.
		 * The added widget will be the new top widget.
		 * @param widget Widget.
		 */
		void Add( std::shared_ptr<Widget> widget );

		/** Remove widget.
		 * @param widget Widget.
		 */
		void Remove( std::shared_ptr<Widget> widget );

		/** Remove all widgets.
		 */
		void RemoveAll();

		/** Refresh all widgets.
		 * All widgets will invalidate and re-request size. This is done
		 * automagically in SetProperty().
		 */
		void Refresh();

		/** Load a style from file.
		 * @param filename Filename.
		 * @return true on success, false otherwise.
		 */
		bool LoadThemeFromFile( const std::string& filename );

		/** Get engine.
		 * @return Engine.
		 */
		Engine& GetEngine();

		/** Bring child to front.
		 * @param child Child.
		 */
		void BringToFront( std::shared_ptr<const Widget> child );

	private:
		typedef std::deque<std::shared_ptr<Widget>> WidgetsList;

		void SendFakeMouseMoveEvent( std::shared_ptr<Widget> widget, int x = -1337, int y = -1337 ) const;
		void RecalculateWidgetLevels();

		mutable Context m_context;
		std::unique_ptr<Engine> m_engine;

		WidgetsList m_children;
		std::weak_ptr<Widget> m_last_receiver;

		sf::Vector2i m_last_mouse_pos;
};

}

#include "Desktop.inl"
