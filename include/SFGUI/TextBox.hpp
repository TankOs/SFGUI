#ifndef SFGUI_TEXTBOX_HPP
#define SFGUI_TEXTBOX_HPP

#include <SFGUI/Widget.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Clock.hpp>

namespace sfg {

/** Text box
 */
class TextBox : public Widget {
	public:
		typedef boost::shared_ptr<TextBox>  Ptr; //!< Shared pointer.
		typedef std::basic_string<sf::Uint32>  u32string; //!< UTF-32 string.

		/** Create button.
		 * @param width Width.
		 * @param text Text.
		 * @return TextBox.
		 */
		static Ptr Create( float width = 40.f, const sf::String& text = L"" );

		/** Set text.
		 * @param text Text.
		 */
		void SetText( const sf::String& text );

		/** Get text.
		 * @return Text.
		 */
		sf::String GetText() const;
		
		/** Get partial string left of cursor.
		 * @return UTF-32 string left of cursor.
		 */
		u32string const& GetLeft() const { return m_left_string; }
		
		/** Get partial string right of cursor.
		 * @return UTF-32 string right of cursor.
		 */
		u32string const& GetRight() const { return m_right_string; }
		
		/** Get visible text.
		 * @return Visible text.
		 */
		sf::String GetVisibleText();
		
		/** Get partial visible string left of cursor.
		 * @return Visible UTF-32 string left of cursor.
		 */
		u32string const GetVisibleLeft();
		
		/** Get partial visible string right of cursor.
		 * @return Visible UTF-32 string right of cursor.
		 */
		u32string const GetVisibleRight();
		
		/** Set border width.
		 * @param width Border width.
		 */
		void SetBorderWidth( float width );
		
		/** Get border width.
		 * @return Border width.
		 */
		float GetBorderWidth() const { return m_border_width; }
		
		/** Get current cursor position.
		 * @return Cursor position.
		 */
		std::size_t GetCursorPos() const;
		
		/** Set cursor position.
		 * @param pos Position.
		 */
		void SetCursorPos( std::size_t pos );
		
		/** Get cursor status.
		 * @return true when cursor is currently being displayed.
		 */
		bool GetCursorStatus() const { return m_cursor_status; }
		
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
		TextBox( float width = 40.f );
		
		/** Get closest cursor position to x coordinate.
		 * @param mouse_pos_x x coordinate.
		 * @return Closest cursor position.
		 */
		std::size_t GetPositionFromMouseX( int mouse_pos_x );
		
		/** Recalculate visible strings.
		 */
		void RecalculateVisibleStrings();
		
		/** Move cursor 1 unit to the left.
		 */
		void MoveCursorLeft();
		
		/** Move cursor 1 unit to the right.
		 */
		void MoveCursorRight();

		void HandleStateChange( Widget::Ptr widget, State oldstate );
		void HandleMouseEnter( Widget::Ptr widget, int x, int y );
		void HandleMouseLeave( Widget::Ptr widget, int x, int y );
		bool HandleMouseButtonClick( Widget::Ptr widget, int x, int y, sf::Mouse::Button button );
		void HandleText( Widget::Ptr widget, sf::Uint32 unicode );
		void HandleKeyPress( Widget::Ptr widget, sf::Event::KeyEvent );
		void HandleFocusChange( Widget::Ptr widget );
		void HandleExpose( Widget::Ptr widget, sf::RenderTarget& target );

		// The cursor is implicitly located between 'left' and 'right'
		u32string m_left_string, m_right_string;
		u32string m_visible_left_string, m_visible_right_string;
		
		float m_width;
		float m_border_width;
		
		sf::Clock m_cursor_timer;
		bool m_cursor_status;
		
		// The UTF-32 character which hides each character of the string
		sf::Uint32 m_text_placeholder;
};

}

#endif // SFGUI_TEXTBOX_HPP

