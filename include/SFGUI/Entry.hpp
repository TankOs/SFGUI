#pragma once
#include <SFGUI/Widget.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Clock.hpp>

namespace sfg {

/** Entry widget
 */
class SFGUI_API Entry : public Widget {
	public:
		typedef std::shared_ptr<Entry>  Ptr; //!< Shared pointer.
		typedef std::shared_ptr<const Entry>  PtrConst; //!< Shared pointer.

		/** Create entry.
		 * @param text Text.
		 * @return Entry.
		 */
		static Ptr Create( const sf::String& text = L"" );

		virtual const std::string& GetName() const;

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
		
		/** Get maximum length.
		 * @return Maximum length (0 if disabled).
		 */
		std::size_t GetMaximumLength() const;
		
		/** Set maximum length.
		 * @param max_length Maximum length (0 to disable).
		 */
		void SetMaximumLength( std::size_t max_length );

		Signal OnTextChanged; //!< Fired when the text changes.

	protected:
		RenderQueue* InvalidateImpl() const;
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
		void RecalculateVisibleString() const;

		/** Move cursor.
		 * @param delta Number of units to move cursor by. Negative to move left. Positive to move right.
		 */
		void MoveCursor( int delta );

		virtual void HandleMouseEnter( int x, int y );
		virtual void HandleMouseLeave( int x, int y );
		virtual void HandleMouseButtonEvent( sf::Mouse::Button button, bool press, int x, int y );
		virtual void HandleTextEvent( sf::Uint32 character );
		virtual void HandleKeyEvent( sf::Keyboard::Key key, bool press );
		virtual void HandleExpose( CullingTarget& target ) const;
		virtual void HandleFocusChange( Widget::Ptr focused_widget );
		virtual void HandleStateChange( State old_state );
		virtual void HandleSizeAllocate( const sf::FloatRect& old_allocation ) const;

		// Data structures holding the total content of the Entry and the visible portion of it
		sf::String m_string;
		mutable sf::String m_visible_string;

		// The offset in the string at which the visible portion starts
		mutable std::size_t m_visible_offset;

		std::size_t m_cursor_position;
		mutable sf::Clock m_cursor_timer;
		mutable bool m_cursor_status;

		// The UTF-32 character which hides each character of the string
		sf::Uint32 m_text_placeholder;
		
		// The maximum text lenght. If it equals to 0, it disables this limit
		std::size_t m_max_length;
};

}
