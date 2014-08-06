#pragma once

#include <SFGUI/Widget.hpp>

#include <SFML/System/String.hpp>
#include <memory>

namespace sfg {

/** Entry widget
 */
class SFGUI_API Entry : public Widget {
	public:
		typedef std::shared_ptr<Entry> Ptr; //!< Shared pointer.
		typedef std::shared_ptr<const Entry> PtrConst; //!< Shared pointer.

		/** Create entry.
		 * @param text Text.
		 * @return Entry.
		 */
		static Ptr Create( const sf::String& text = L"" );

		const std::string& GetName() const override;

		/** Set text.
		 * @param text Text.
		 */
		void SetText( const sf::String& text );

		/** Get text.
		 * @return Text.
		 */
		const sf::String& GetText() const;

		/** Append text.
		 * @param text Text.
		 */
		void AppendText( const sf::String& text );

		/** Prepend text.
		 * @param text Text.
		 */
		void PrependText( const sf::String& text );

		/** Get visible text.
		 * @return Visible text.
		 */
		const sf::String& GetVisibleText() const;

		/** Get offset to the beginning of the visible text.
		 * @return Offset.
		 */
		int GetVisibleOffset() const;

		/** Get current cursor position.
		 * @return Cursor position.
		 */
		int GetCursorPosition() const;

		/** Set cursor position.
		 * @param pos Position.
		 */
		void SetCursorPosition( int pos );

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
		int GetMaximumLength() const;

		/** Set maximum length.
		 * @param max_length Maximum length (0 to disable).
		 */
		void SetMaximumLength( int max_length );

		// Signals.
		static Signal::SignalID OnTextChanged; //!< Fired when the text changes.

	protected:
		/** Ctor.
		 */
		Entry();

		/** Set right hand side margin that the entry should reserve for derived widgets.
		 * @param margin Margin to reserve.
		 */
		void SetTextMargin( float margin );

		std::unique_ptr<RenderQueue> InvalidateImpl() const override;
		sf::Vector2f CalculateRequisition() override;

		void HandleMouseButtonEvent( sf::Mouse::Button button, bool press, int x, int y ) override;
		void HandleUpdate( float seconds ) override;
		void HandleTextEvent( sf::Uint32 character ) override;
		void HandleKeyEvent( sf::Keyboard::Key key, bool press ) override;
		void HandleSizeChange() override;
		void HandleFocusChange( Widget::Ptr focused_widget ) override;

	private:
		/** Get closest cursor position to x coordinate.
		 * @param mouse_pos_x x coordinate.
		 * @return Closest cursor position.
		 */
		int GetPositionFromMouseX( int mouse_pos_x );

		/** Recalculate visible string.
		 */
		void RecalculateVisibleString() const;

		/** Move cursor.
		 * @param delta Number of units to move cursor by. Negative to move left. Positive to move right.
		 */
		void MoveCursor( int delta );

		void HandleMouseEnter( int x, int y ) override;
		void HandleMouseLeave( int x, int y ) override;

		// Data structures holding the total content of the Entry and the visible portion of it
		sf::String m_string;
		mutable sf::String m_visible_string;

		// The offset in the string at which the visible portion starts
		mutable int m_visible_offset;

		// The UTF-32 character which hides each character of the string
		sf::Uint32 m_text_placeholder;

		// The maximum text length. If it equals to 0, it disables this limit
		int m_max_length;

		int m_cursor_position;

		float m_elapsed_time;

		float m_text_margin;

		mutable bool m_cursor_status;
};

}
