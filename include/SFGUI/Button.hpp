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

		/** Set caption.
		 * @param caption Caption.
		 */
		void SetCaption( const sf::String& caption );

		/* Get caption.
		 * @return Caption.
		 */
		const sf::String& GetCaption() const;

	protected:
		sf::Drawable* InvalidateImpl();

	private:
		Button();

		sf::String  m_caption;
};

}
