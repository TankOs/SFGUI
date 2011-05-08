#ifndef SFGUI_TEXTBOX_HPP
#define SFGUI_TEXTBOX_HPP

#include <SFGUI/Widget.hpp>
#include <SFML/System/String.hpp>

namespace sfg {

/** Text box
 */
class TextBox : public Widget {
	public:
		typedef boost::shared_ptr<TextBox>  Ptr; //!< Shared pointer.
		typedef std::basic_string<sf::Uint32> u32string;

		/** Create button.
		 * @param text Text.
		 * @return TextBox.
		 */
		static Ptr Create( float width = 40.f, const sf::String& Text = L"" );

		/** Set text.
		 * @param text Text.
		 */
		void SetText( const sf::String& text );

		/* Get text.
		 * @return Text.
		 */
		sf::String GetText() const;

		u32string const& GetLeft() const { return left; }
		u32string const& GetRight() const { return right; }

		void SetBorderWidth(float w);
		float GetBorderWidth() const { return borderWidth; }

		std::size_t GetCursorPos() const;

		void SetCursorPos(std::size_t const pos);

		Signal<void( Widget::Ptr )> TextChanged; //!< Fired when the text changes.

	protected:
		sf::Drawable* InvalidateImpl( const sf::RenderTarget& target );
		sf::Vector2f GetRequisitionImpl() const;

	private:
		TextBox(float width = 40.f);

		void HandleStateChange( Widget::Ptr widget, State oldstate );
		void HandleMouseEnter( Widget::Ptr widget, int x, int y );
		void HandleMouseLeave( Widget::Ptr widget, int x, int y );
		bool HandleMouseButtonClick( Widget::Ptr widget, int x, int y, sf::Mouse::Button button );
		void HandleText( Widget::Ptr widget, sf::Uint32 unicode );
		void HandleKeyRelease( Widget::Ptr widget, sf::Event::KeyEvent );

		// The cursor is implicitly located between 'left' and 'right'
		u32string left, right;
		float width;
		float borderWidth;
};

}

#endif // SFGUI_TEXTBOX_HPP

