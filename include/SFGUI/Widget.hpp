#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Signal.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>
#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/scoped_ptr.hpp>
#include <map>
#include <list>

namespace sfg {

/** Base class for widgets.
 */
class SFGUI_API Widget : public boost::noncopyable, public boost::enable_shared_from_this<Widget> {
	public:
		typedef boost::shared_ptr<Widget>  Ptr; //!< Shared pointer.

		/** Widget state.
		 */
		enum State {
			Normal = 0, /*!< Normal. */
			Active, /*!< Active, e.g. when a button is pressed. */
			Prelight, /*!< Prelight, e.g. when the mouse moves over a widget. */
			Selected, /*!< Selected, e.g. when a list item in a list is selected. */
			Insensitive /*!< Insensitive, disabled widget. */
		};

		/** Event handling result.
		 * The result gives the caller information about what happened with the
		 * event and how to go on. It can either be dropped (e.g. when a mouse move
		 * occurs outside the widget) so that it won't get passed to children,
		 * passed (e.g. a mouse move is inside the widget but no handler processed
		 * it so that further processing should be continued for the children) or
		 * eaten (e.g. button has been clicked, so no further event processing
		 * should take place *at all*).
		 */
		enum HandleEventResult {
			PassEvent = 0,
			EatEvent,
			DropEvent
		};

		/** Destructor.
		 */
		virtual ~Widget();

		/** Check if widget is sensitive (enabled).
		 * @return true when sensitive.
		 */
		bool IsSensitive() const;

		/** Check if widget is visible, i.e. gets rendered.
		 * @return true when visible.
		 */
		bool IsVisible() const;

		/** Show (or hide) widget.
		 * @param show true to show, false to hide.
		 */
		void Show( bool show = true );

		/** Check if widget has the focus.
		 * @return true when focussed.
		 */
		bool HasFocus() const;

		/** Set name of widget.
		 * Used to identify the widget to find it in containers, for example.
		 * @param name Name.
		 */
		void SetName( const std::string& name );

		/** Get name of widget.
		 * @return Name.
		 */
		const std::string& GetName() const;

		/** Grab focus.
		 */
		void GrabFocus();

		/** Allocate size.
		 * @param rect Rect.
		 */
		void AllocateSize( const sf::FloatRect& rect );

		/** Request new allocation at parent.
		 */
		void RequestSize();

		/** Get allocated size (position and size).
		 * @return Rect.
		 */
		const sf::FloatRect& GetAllocation() const;

		/** Get requested size (requisition).
		 * @return Size.
		 */
		virtual sf::Vector2f GetRequisition() const = 0;

		/** Set position.
		 * @param position Position.
		 */
		void SetPosition( const sf::Vector2f& position );

		/** Expose.
		 * Render widget to given target.
		 * @param target SFML render target.
		 */
		void Expose( sf::RenderTarget& target );

		/** Invalidate widget (prepare internal sf::Drawable).
		 * Implement InvalidateImpl() for your own code.
		 */
		void Invalidate();

		/** Set parent widget.
		 * Note that the parent must be a subclass of sfg::Container. You mostly
		 * don't want to call this method directly.
		 * @param parent Parent.
		 */
		void SetParent( Widget::Ptr parent );

		/** Get parent.
		 * @return Parent.
		 */
		Ptr GetParent() const;

		/** Set widget's state.
		 * @param state State.
		 */
		void SetState( State state );

		/** Get widget's state.
		 * @return State.
		 */
		State GetState() const;

		/** Handle SFML event.
		 * Handle an SFML event and fire proper signals. Normally reimplemented by
		 * containers only.
		 * @return true when event has been processed (eaten).
		 */
		virtual HandleEventResult HandleEvent( const sf::Event& event );

		// Signals.
		Signal<void( Ptr, State )>  OnStateChange; //!< Fired when state changed. (old state)
		Signal<void( Ptr )>         OnFocusChange; //!< Fired when focus grabbed or lost.

		Signal<void( Ptr, sf::RenderTarget& )>  OnExpose; //!< Fired when widget is being rendered.

		Signal<void( Ptr, const sf::FloatRect& )>  OnSizeAllocate; //!< Fired when widget's allocation changed.
		Signal<void( Ptr, const sf::Vector2f& )>   OnSizeRequest; //!< Fired when requested a new widget's size.

		Signal<void( Ptr, int, int )>  OnMouseEnter; //!< Fired when mouse entered widget. (x, y)
		Signal<void( Ptr, int, int )>  OnMouseLeave; //!< Fired when mouse left widget. (x, y)
		Signal<void( Ptr, int, int )>  OnMouseMove; //!< Fired when mouse moved over widget. (x, y)
		Signal<void( Ptr, int, int, sf::Mouse::Button )>  OnMouseButtonPress; //!< Fired when mouse button pressed. (x, y, button)
		Signal<void( Ptr, int, int, sf::Mouse::Button )>  OnMouseButtonRelease; //!< Fired when mouse button released. (x, y, button)
		Signal<void( Ptr, int, int, sf::Mouse::Button )>  OnMouseButtonClick; //!< Fired when mouse button clicked (pressed and released in same widget). (x, y, button)

	protected:
		/** Constructor.
		 */
		Widget();

		/** Invalidate implementation (redraw internally).
		 * Gets only called when a rendering engine has been set.
		 * @return Pointer to new drawable -- ownership is taken by caller.
		 */
		virtual sf::Drawable* InvalidateImpl();

		/** Register event hook.
		 * Widgets that register an event hook get notifications of the proper
		 * event type no matter if it fits or not. Mainly used to track the mouse
		 * pointer when it leaves a widget.
		 * @param event_type Type of event.
		 * @param widget Widget that shall receive the events.
		 */
		void RegisterEventHook( sf::Event::EventType event_type, Ptr widget );

		/** Unregister event hook.
		 * @param event_type Type of event.
		 * @param widget Widget that has previously registered the hook.
		 */
		void UnregisterEventHook( sf::Event::EventType event_type, Ptr widget );

		/** Check if mouse is inside widget.
		 * @return true if mouse is inside.
		 */
		bool IsMouseInWidget() const;

	private:
		struct WidgetBoolPair {
			WidgetBoolPair( Ptr widget_, bool remove_ );
			bool operator==( const WidgetBoolPair& rhs );
			bool operator==( const Ptr& rhs );
			Ptr  widget;
			bool  remove;
		};

		typedef std::list<WidgetBoolPair>  WidgetsList;
		typedef std::map<sf::Event::EventType, WidgetsList>  HooksMap;

		void GrabFocus( Ptr widget );

		Ptr  m_parent;

		bool  m_sensitive;
		bool  m_visible;
		Ptr   m_focus_widget;

		State  m_state;
		bool  m_mouse_in;
		int  m_mouse_button_down;

		std::string    m_name;
		sf::FloatRect  m_allocation;
		sf::Vector2f   m_requisition;

		bool  m_invalidated;

		HooksMap  m_hooks;

		mutable boost::scoped_ptr<sf::Drawable>  m_drawable;
};

}
