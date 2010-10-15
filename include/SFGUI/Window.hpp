#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Bin.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/String.hpp>
#include <boost/scoped_ptr.hpp>

namespace sfg {

/** Window.
 * TODO: Remove slot when child gets removed.
 */
class SFGUI_API Window : public Bin {
	public:
		typedef boost::shared_ptr<Window>  Ptr; //!< Shared pointer.

		/** Create widget.
		 */
		static Ptr Create();

		/** Set window title.
		 * @param title Title.
		 */
		void SetTitle( const sf::String& title );

		/** Get window title.
		 * @return Title.
		 */
		const sf::String& GetTitle() const;

		/** Get client area.
		 * @return Rect.
		 */
		sf::FloatRect GetClientRect() const;

		void QueueResize( Widget::Ptr widget );

	protected:
		/** Constructor.
		 */
		Window();

		virtual sf::Drawable* InvalidateImpl();

	private:
		void HandleAdd( Widget::Ptr widget, Widget::Ptr child );
		void HandleSizeAllocate( Widget::Ptr widget, const sf::FloatRect& oldallocation );

		sf::String  m_title;
		bool  m_skipreallocation;
};

}
