#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Bin.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/String.hpp>
#include <boost/scoped_ptr.hpp>

namespace sfg {

/** Window.
 */
class SFGUI_API Window : public Bin {
	public:
		typedef boost::shared_ptr<Window>  Ptr; //!< Shared pointer.

		/** Create widget.
		 * @param rect Initial rect.
		 */
		static Ptr Create( const sf::FloatRect& rect = sf::FloatRect( 0, 0, 0, 0 ) );

		/** Set window title.
		 * @param title Title.
		 */
		void SetTitle( const sf::String& title );

		/** Get window title.
		 * @return Title.
		 */
		const sf::String& GetTitle() const;

	protected:
		/** Constructor.
		 */
		Window();

		virtual sf::Drawable* InvalidateImpl();

	private:
		sf::String  m_title;
};

}
