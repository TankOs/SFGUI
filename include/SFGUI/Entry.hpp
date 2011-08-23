#pragma once
#include <SFGUI/Widget.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Clock.hpp>

namespace sfg {

/** Entry widget
 */
class Entry : public Widget {
	public:
		typedef boost::shared_ptr<Entry>  Ptr; //!< Shared pointer.

		/** Create entry.
		 * @param width Width.
		 * @param text Text.
		 * @return Entry.
		 */
		static Ptr Create( float width = 40.f, const sf::String& text = L"" );

		/** Set text.
		 * @param text Text.
		 */
		void SetText( const sf::String& text );

		/** Get text.
		 * @return Text.
		 */
		sf::String GetText() const { return m_string; }

		/** Get visible text.
		 * @return Visible text.
		 */
		sf::String GetVisibleText() const { return m_visible_string; }

		/** Get current cursor position.
		 * @return Cursor position.
		 */
		std::size_t GetCursorPos() const { return m_cursor_position; }
		
		/** Set cursor position.
		 * @param pos Position.
		 */
		void SetCursorPos( std::size_t pos );
		
		/** Get cursor status.
		 * @return true when cursor is currently being displayed.
		 */
		bool GetCursorStatus() const { return m_cursor_status; }
		
		/** Get distance from left side of the widget at which the cursor is rendered.
		 * @return distance from left side of the widget at which the cursor is rendered.
		 */
		float GetCursorOffset() const { return m_cursor_offset; }

		/** Hide all characters of the string with the given character.
		 * @param c UTF-32 Character ( 0 to not hide text ).
		 */
		void HideText( sf::Uint32 c );
		
		/** Get the character that hides all characters of the string.
		 * @return UTF-32 Character.
		 */
		sf::Uint32 GetHideChar() const { return m_text_placeholder; }

		Signal<void( Widget::Ptr )>  OnTextChanged; //!< Fired when the text changes.

	protected:
		sf::Drawable* InvalidateImpl( const sf::RenderTarget& target );
		sf::Vector2f GetRequisitionImpl() const;

	private:
		/** Ctor.
		 * @param width Width.
		 */
		Entry( float width = 40.f );
		
		/** Get closest cursor position to x coordinate.
		 * @param mouse_pos_x x coordinate.
		 * @return Closest cursor position.
		 */
		std::size_t GetPositionFromMouseX( int mouse_pos_x );
		
		/** Recalculate visible string.
		 */
		void RecalculateVisibleString();
		
		/** Move cursor.
		 * @param delta Number of units to move cursor by. Negative to move left. Positive to move right.
		 */
		void MoveCursor( int delta );

		void HandleStateChange( Widget::Ptr widget, State oldstate );
		void HandleMouseEnter( Widget::Ptr widget, int x, int y );
		void HandleMouseLeave( Widget::Ptr widget, int x, int y );
		bool HandleMouseButtonClick( Widget::Ptr widget, int x, int y, sf::Mouse::Button button );
		void HandleText( Widget::Ptr widget, sf::Uint32 unicode );
		void HandleKeyPress( Widget::Ptr widget, sf::Event::KeyEvent );
		void HandleFocusChange( Widget::Ptr widget );
		void HandleExpose( Widget::Ptr widget, sf::RenderTarget& target );

		// Data structures holding the total content of the Entry and the visible portion of it
		sf::String m_string;
		sf::String m_visible_string;

		// The offset in the string at which the visible portion starts
		std::size_t m_visible_offset;

		// Widget width
		float m_width;
		
		std::size_t m_cursor_position;
		sf::Clock m_cursor_timer;
		bool m_cursor_status;
		float m_cursor_offset;
		
		// The UTF-32 character which hides each character of the string
		sf::Uint32 m_text_placeholder;
};

}
