#pragma once
#include <SFGUI/Widget.hpp>
#include <SFML/System/String.hpp>

namespace sfg {

/** Pushbutton.
 */
class Button : public Widget {
	public:
		typedef boost::shared_ptr<Button>  Ptr; //!< Shared pointer.

		/** Create button.
		 * @param caption Caption.
		 * @return Button.
		 */
		static Ptr Create( const sf::String& caption = L"" );

		sf::Vector2f GetRequisition() const;

		/** Set caption.
		 * @param caption Caption.
		 */
		void SetCaption( const sf::String& caption );

		/* Get caption.
		 * @return Caption.
		 */
		const sf::String& GetCaption() const;

		Signal<void( Widget::Ptr )>  OnClick; //!< Fired when button clicked.

	protected:
		sf::Drawable* InvalidateImpl();

	private:
		Button();

		void HandleStateChange( sfg::Widget::Ptr widget, State oldstate );
		void HandleMouseEnter( sfg::Widget::Ptr widget, int x, int y );
		void HandleMouseLeave( sfg::Widget::Ptr widget, int x, int y );
		void HandleMouseButtonClick( sfg::Widget::Ptr widget, int x, int y, sf::Mouse::Button button );
		void HandleMouseButtonPress( sfg::Widget::Ptr widget, int x, int y, sf::Mouse::Button button );
		void HandleMouseButtonRelease( sfg::Widget::Ptr widget, int x, int y, sf::Mouse::Button button );

		sf::String  m_caption;
};

}
