#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/View.hpp>
#include <memory>
#include <string>
#include <list>

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
		/** Ctor.
		 * @param viewport Viewport.
		 */
		Desktop( const sf::FloatRect& viewport );

		/** Ctor.
		 * @param window Window which size is used for the desktop's viewport.
		 */
		Desktop( const sf::Window& window );

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

		/** Get property.
		 * @param property Name of property.
		 * @param widget Widget to be used for building the property path.
		 * @return Value or T() in case property doesn't exist.
		 */
		template <typename T>
		T GetProperty( const std::string& property, std::shared_ptr<const Widget> widget = std::shared_ptr<const Widget>() ) const;

		/** Expose.
		 * @param target Rendering target.
		 */
		void Expose( sf::RenderTarget& target ) const;

		/** Expose.
		 * @param target Rendering target.
		 */
		void Expose( CullingTarget& target ) const;

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

		/** Refresh all widgets.
		 * All widgets will invalidate and re-request size. This is done
		 * automagically in SetProperty().
		 */
		void Refresh() const;

		/** Load a style from file.
		 * @param filename Filename.
		 * @return true on success, false otherwise.
		 */
		bool LoadThemeFromFile( const std::string& filename );

		/** Get engine.
		 * @return Engine.
		 */
		Engine& GetEngine();

	private:
		typedef std::list<std::shared_ptr<Widget> > WidgetsList;

		sf::Vector2f TransformToLocal( const sf::Vector2f& global ) const;
		void RemoveObsoleteChildren();

		sf::View m_view;

		mutable Context m_context;
		std::unique_ptr<Engine> m_engine;

		WidgetsList m_children;
		mutable WidgetsList m_obsolete_children;
		std::weak_ptr<Widget> m_last_receiver;

		mutable bool m_do_refresh;
};

}

#include "Desktop.inl"
