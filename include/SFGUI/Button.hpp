#pragma once
#include <SFGUI/Bin.hpp>
#include <SFGUI/Label.hpp>
#include <SFML/System/String.hpp>

namespace sfg {

/** Pushbutton.
 */
class SFGUI_API Button : public Bin {
	public:
		typedef boost::shared_ptr<Button>  Ptr; //!< Shared pointer.
		typedef boost::shared_ptr<const Button>  PtrConst; //!< Shared pointer.

		/** Create button.
		 * @param label Label.
		 * @return Button.
		 */
		static Ptr Create( const sf::String& label = L"" );

		/** Set label.
		 * @param label Label.
		 */
		void SetLabel( const sf::String& label );

		/* Get label.
		 * @return Label.
		 */
		const sf::String& GetLabel() const;

		/** Set padding.
		 * @param padding Padding.
		 */
		void SetPadding( float padding );

		/** Get padding.
		 * @return Padding.
		 */
		float GetPadding() const;

		Signal<void( Widget::Ptr )>  OnClick; //!< Fired when button clicked.

	protected:
		sf::Drawable* InvalidateImpl();
		sf::Vector2f GetRequisitionImpl() const;

	private:
		Button();

		void HandleStateChange( Widget::Ptr widget, State oldstate );
		void HandleMouseEnter( Widget::Ptr widget, int x, int y );
		bool HandleMouseLeave( Widget::Ptr widget, int x, int y );
		bool HandleMouseButtonClick( Widget::Ptr widget, int x, int y, sf::Mouse::Button button );
		bool HandleMouseButtonPress( Widget::Ptr widget, int x, int y, sf::Mouse::Button button );
		bool HandleMouseButtonRelease( Widget::Ptr widget, int x, int y, sf::Mouse::Button button );
		void HandleSizeAllocate( Widget::Ptr widget, const sf::FloatRect& old_allocation );

		Label::Ptr  m_label;
		float m_padding;
};

}
