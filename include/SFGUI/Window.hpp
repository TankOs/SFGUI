#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Bin.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/String.hpp>
#include <boost/scoped_ptr.hpp>

namespace sfg {

/** Window.
 */
class SFGUI_API Window : public Bin {
	public:
		typedef boost::shared_ptr<Window>  Ptr; //!< Shared pointer.
		typedef boost::shared_ptr<const Window>  PtrConst; //!< Shared pointer.

		enum Style {
			NoStyle = 0, //!< Transparent window.
			Titlebar = 1 << 0, //!< Titlebar.
			Background = 1 << 1, //!< Titlebar.
			Resize = 1 << 2, //!< Resizable.
			Toplevel = Titlebar | Background | Resize //!< Toplevel window.
		};

		/** Create widget.
		 */
		static Ptr Create();

		virtual const std::string& GetName() const;

		/** Set window title.
		 * @param title Title.
		 */
		void SetTitle( const sf::String& title );

		/** Get window title.
		 * @return Title.
		 */
		const sf::String& GetTitle() const;

		/** Get client area.
		 * @return Rect.
		 */
		sf::FloatRect GetClientRect() const;

		/** Set window style.
		 * Can be a combination of Window::Style values.
		 * @param style New style.
		 */
		void SetStyle( int style );

		/** Get window style.
		 * @return Window style.
		 */
		int GetStyle() const;

		/** Check if the window has a specific style.
		 * @param style Style to check.
		 * @return true when window has desired style.
		 */
		bool HasStyle( Style style ) const;

	protected:
		/** Constructor.
		 */
		Window();

		virtual sf::Drawable* InvalidateImpl();

		sf::Vector2f GetRequisitionImpl() const;

	private:
		void HandleAdd();
		void HandleSizeAllocate();
		void HandleDragMove();

		sf::String  m_title;
		bool  m_skipreallocation;
		int  m_style;
};

}
