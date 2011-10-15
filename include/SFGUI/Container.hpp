#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Widget.hpp>
#include <list>

namespace sfg {

/**
 * Base class for container-like widgets.
 */
class SFGUI_API Container : public Widget {
	public:
		typedef boost::shared_ptr<Container>  Ptr; //!< Shared pointer.
		typedef boost::shared_ptr<const Container>  PtrConst; //!< Shared pointer.
		typedef std::list<Widget::Ptr>  WidgetsList;

		/** Pack options.
		 */
		enum PackOptions {
			EXPAND = 1 << 0, ///< Expand widget to maximum possible size.
			FILL = 1 << 1 ///< Fill widget to allocated size.
		};

		/** Add child.
		 * @param widget Widget to add.
		 */
		void Add( Widget::Ptr widget );

		/** Remove child (from container).
		 * @param widget Widget to remove.
		 */
		void Remove( Widget::Ptr widget );

		/** Check if a widget is a child of this container.
		 * @param widget Widget to search for.
		 */
		bool IsChild( Widget::Ptr widget ) const;

		/** Get children.
		 * @return std::list with children.
		 */
		const WidgetsList& GetChildren() const;

		/** Set border width.
		 * @param width Border width.
		 */
		void SetBorderWidth( float width );

		/** Get border width.
		 * @return Border width.
		 */
		float GetBorderWidth() const;

		/** Handle SFML event.
		 * Handle an SFML event and fire proper signals.
		 * @return true when event has been processed (eaten).
		 */
		virtual HandleEventResult HandleEvent( const sf::Event& event );

		/** Register event hook.
		 * Widgets that register an event hook get notifications of the proper
		 * event type no matter if it fits or not. Mainly used to track the mouse
		 * pointer when it leaves a widget.
		 * @param event_type Type of event.
		 * @param widget Widget that shall receive the events.
		 */
		void RegisterEventHook( sf::Event::EventType event_type, Widget::Ptr widget );

		/** Unregister event hook.
		 * @param event_type Type of event.
		 * @param widget Widget that has previously registered the hook.
		 */
		void UnregisterEventHook( sf::Event::EventType event_type, Widget::Ptr widget );

		Signal<void( Widget::Ptr, Widget::Ptr )>  OnAdd; //!< Fired when child added.
		Signal<void( Widget::Ptr, Widget::Ptr )>  OnRemove; //!< Fired when child removed.

	protected:
		/** Constructor.
		 */
		Container();

		boost::signals2::scoped_connection m_expose_connection;

	private:
		struct WidgetBoolPair {
			WidgetBoolPair( Widget::Ptr widget_, bool remove_ );
			bool operator==( const WidgetBoolPair& rhs );
			bool operator==( const Widget::Ptr& rhs );
			Widget::Ptr  widget;
			bool  remove;
		};

		typedef std::list<WidgetBoolPair>  HookedWidgetsList;
		typedef std::map<sf::Event::EventType, HookedWidgetsList>  HooksMap;

		HandleEventResult ProcessHooks( const sf::Event& event );

		void HandleExpose( Widget::Ptr widget, sf::RenderTarget& target );
		void HandleSizeAllocate( Widget::Ptr widget, const sf::FloatRect& oldallocation );
		void HandlePositionChange( Widget::Ptr widget, const sf::FloatRect& oldallocation );

		float  m_border_width;

		WidgetsList  m_children;
		HooksMap  m_hooks;

};

}
