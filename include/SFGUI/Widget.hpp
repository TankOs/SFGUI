#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Signal.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <string>

namespace sfg {

/** Base class for widgets.
 */
class SFGUI_API Widget : public boost::noncopyable, public boost::enable_shared_from_this<Widget> {
	public:
		typedef boost::shared_ptr<Widget>  Ptr; // Shared pointer to Widget.

		/** Widget state.
		 */
		enum State {
			Normal = 0, // Normal.
			Active, // Active, e.g. when a button is pressed.
			Prelight, // Prelight, e.g. when the mouse moves over a widget.
			Selected, // Selected, e.g. when a list item in a list is selected.
			Insensitive // Insensitive, disabled widget.
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

		Signal<void( Ptr )>         OnEnter; // Fired when mouse entered widget.
		Signal<void( Ptr )>         OnLeave; // Fired when mouse left widget.
		Signal<void( Ptr, State )>  OnStateChange; // Fired when state changed.
		Signal<void( Ptr )>         OnFocusChange; // Fired when focus grabbed or lost.

	private:
		Widget();

		bool  m_sensitive;
		bool  m_visible;
		bool  m_focus;

		State  m_state;

		std::string  m_name;
};

}
