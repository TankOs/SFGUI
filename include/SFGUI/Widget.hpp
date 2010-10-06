#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Signal.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>
#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/scoped_ptr.hpp>
#include <string>

namespace sfg {

class RenderEngine;

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

		/** Get rect (position and size).
		 * @return Rect.
		 */
		const sf::FloatRect& GetRect() const;

		/** Set render engine.
		 * @param engine Engine. (pointer gets stored)
		 */
		void SetRenderEngine( const RenderEngine& engine );

		/** Get current render engine.
		 * @return Pointer to engine or 0 if none.
		 */
		const RenderEngine* GetRenderEngine() const;

		/** Expose.
		 * Renders widget to given target.
		 * @param target SFML render target.
		 */
		void Expose( sf::RenderTarget& target ) const;

		/** Invalidate widget (redraw internally).
		 * Implement InvalidateImpl() for your own code.
		 */
		void Invalidate();

		// Signals.
		Signal<void( Ptr, State )>  OnStateChange; //!< Fired when state changed. (new state)
		Signal<void( Ptr )>         OnFocusChange; //!< Fired when focus grabbed or lost.
		Signal<void( Ptr )>         OnConfigure; //!< Fired when widget's size changed.

		Signal<void( Ptr )>         OnMouseEnter; //!< Fired when mouse entered widget.
		Signal<void( Ptr )>         OnMouseLeave; //!< Fired when mouse left widget.
		Signal<bool( Ptr, int, int, sf::Mouse::Button )>  OnMouseButtonPress; //!< Fired when mouse button pressed. (x, y, button)
		Signal<bool( Ptr, int, int, sf::Mouse::Button )>  OnMouseButtonRelease; //!< Fired when mouse button released. (x, y, button)

	protected:
		/** Constructor.
		 */
		Widget();

		/** Internal grab focus method.
		 * The request is delivered to the highest widget in the current hierarchy.
		 * @param widget Widget that requested focus.
		 */
		void GrabFocus( Ptr widget );

		/** Invalidate implementation (redraw internally).
		 * Gets only called when a rendering engine has been set.
		 * @return Pointer to new drawable -- ownership is taken by caller.
		 */
		virtual sf::Drawable* InvalidateImpl();

	private:
		Ptr  m_parent;

		bool  m_sensitive;
		bool  m_visible;
		Ptr   m_focus_widget;

		State  m_state;

		std::string    m_name;
		sf::FloatRect  m_rect;

		const RenderEngine*  m_renderengine;
		mutable boost::scoped_ptr<sf::Drawable>  m_drawable;
};

}
