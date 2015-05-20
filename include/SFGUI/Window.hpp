#pragma once

#include <SFGUI/Bin.hpp>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>

namespace sfg {

/** Window.
 */
class SFGUI_API Window : public Bin {
	public:
		typedef std::shared_ptr<Window> Ptr; //!< Shared pointer.
		typedef std::shared_ptr<const Window> PtrConst; //!< Shared pointer.

		enum Style : char {
			NO_STYLE = 0, //!< Transparent window.
			TITLEBAR = 1 << 0, //!< Titlebar.
			BACKGROUND = 1 << 1, //!< Background.
			RESIZE = 1 << 2, //!< Resizable.
			SHADOW = 1 << 3, //!< Display Shadow.
			CLOSE = 1 << 4, //!< Display close button.
			TOPLEVEL = TITLEBAR | BACKGROUND | RESIZE //!< Toplevel window.
		};

		/** Create window.
		 * @param style Style the Window should have. Defaults to TopLevel.
		 */
		static Ptr Create( char style = Style::TOPLEVEL );

		const std::string& GetName() const override;

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
		void SetStyle( char style );

		/** Get window style.
		 * @return Window style.
		 */
		char GetStyle() const;

		/** Check if the window has a specific style.
		 * @param style Style to check.
		 * @return true when window has desired style.
		 */
		bool HasStyle( Style style ) const;

		// Signals.
		static Signal::SignalID OnCloseButton; //!< Fired when close button is pressed.

	protected:
		/** Constructor.
		 * @param style Window style.
		 */
		Window( char style );

		std::unique_ptr<RenderQueue> InvalidateImpl() const override;

		sf::Vector2f CalculateRequisition() override;

	private:
		void HandleSizeChange() override;
		void HandleMouseButtonEvent( sf::Mouse::Button button, bool press, int x, int y ) override;
		void HandleMouseMoveEvent( int x, int y ) override;
		bool HandleAdd( Widget::Ptr child ) override;

		sf::Vector2f m_drag_offset;

		sf::String m_title;
		char m_style;

		bool m_dragging;
		bool m_resizing;
};

}
