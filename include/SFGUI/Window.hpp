#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Bin.hpp>
#include <SFGUI/SharedPtr.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/String.hpp>

namespace sfg {

/** Window.
 */
class SFGUI_API Window : public Bin {
	public:
		typedef SharedPtr<Window> Ptr; //!< Shared pointer.
		typedef SharedPtr<const Window> PtrConst; //!< Shared pointer.

		enum Style {
			NO_STYLE = 0, //!< Transparent window.
			TITLEBAR = 1 << 0, //!< Titlebar.
			BACKGROUND = 1 << 1, //!< Background.
			RESIZE = 1 << 2, //!< Resizable.
			SHADOW = 1 << 3, //!< Display Shadow.
			TOPLEVEL = TITLEBAR | BACKGROUND | RESIZE //!< Toplevel window.
		};

		/** Dtor.
		 */
		virtual ~Window();

		/** Create window.
		 * @param style Style the Window should have. Defaults to TopLevel.
		 */
		static Ptr Create( int style = TOPLEVEL );

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
		 * @param style Window style.
		 */
		Window( int style );

		virtual RenderQueue* InvalidateImpl() const;

		sf::Vector2f CalculateRequisition();

	private:
		void HandleSizeChange();
		void HandleMouseButtonEvent( sf::Mouse::Button button, bool press, int x, int y );
		void HandleMouseMoveEvent( int x, int y );
		void HandleAdd( const Widget::Ptr& child );

		sf::Vector2f m_drag_offset;

		sf::String m_title;
		int m_style;

		bool m_dragging;
		bool m_resizing;
};

}
