#pragma once
#include <SFGUI/Widget.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Clock.hpp>

namespace sfg {

/** Entry widget
 */
class SFGUI_API Entry : public Widget {
	public:
		typedef boost::shared_ptr<Entry>  Ptr; //!< Shared pointer.

		/** Create entry.
		 * @param text Text.
		 * @return Entry.
		 */
		static Ptr Create( const sf::String& text = L"" );

		/** Set text.
		 * @param text Text.
		 */
		void SetText( const sf::String& text );

		/** Get text.
		 * @return Text.
		 */
		const sf::String& GetText() const;

		/** Get visible text.
		 * @return Visible text.
		 */
		const sf::String& GetVisibleText() const;

		/** Get offset to the beginning of the visible text.
		 * @return Offset.
		 */
		std::size_t GetVisibleOffset() const;

		/** Get current cursor position.
		 * @return Cursor position.
		 */
		std::size_t GetCursorPosition() const;

		/** Set cursor position.
		 * @param pos Position.
		 */
		void SetCursorPosition( std::size_t pos );

		/** Check if cursor is visible.
		 * @return true if visible./
		 */
		bool IsCursorVisible() const;

		/** Hide all characters of the string with the given character.
		 * @param c UTF-32 Character (0 to not hide text).
		 */
		void HideText( sf::Uint32 c );

		/** Get the character that hides all characters of the string.
		 * @return UTF-32 Character.
		 */
		sf::Uint32 GetHideCharacter() const;

		Signal<void( Widget::Ptr )> OnTextChanged; //!< Fired when the text changes.

	protected:
		sf::Drawable* InvalidateImpl();
		sf::Vector2f GetRequisitionImpl() const;

	private:
		/** Ctor.
		 */
		Entry();

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
		bool HandleMouseButtonPress( Widget::Ptr widget, int x, int y, sf::Mouse::Button button );
		void HandleText( Widget::Ptr widget, sf::Uint32 unicode );
		void HandleKeyPress( Widget::Ptr widget, sf::Event::KeyEvent );
		void HandleFocusChange( Widget::Ptr widget );
		void HandleExpose( Widget::Ptr widget, sf::RenderTarget& target );

		// Data structures holding the total content of the Entry and the visible portion of it
		sf::String m_string;
		sf::String m_visible_string;

		// The offset in the string at which the visible portion starts
		std::size_t m_visible_offset;

		std::size_t m_cursor_position;
		sf::Clock m_cursor_timer;
		bool m_cursor_status;

		// The UTF-32 character which hides each character of the string
		sf::Uint32 m_text_placeholder;
};

}
