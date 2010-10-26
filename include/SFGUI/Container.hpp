#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Widget.hpp>
#include <list>

namespace sfg {

/**
 * Base class for container-like widgets.
 */
class SFGUI_API Container : public Widget {
	public:
		typedef boost::shared_ptr<Container>  Ptr; //!< Shared pointer.
		typedef std::list<Widget::Ptr>  WidgetsList;

		/** Add child.
		 * @param widget Widget to add.
		 */
		void Add( Widget::Ptr widget );

		/** Remove child (from container).
		 * @param widget Widget to remove.
		 */
		void Remove( Widget::Ptr widget );

		/** Check if a widget is a child of this container.
		 * @param widget Widget to search for.
		 */
		bool IsChild( Widget::Ptr widget ) const;

		/** Get children.
		 * @return std::set with children.
		 */
		const WidgetsList& GetChildren() const;

		/** Set border width.
		 * @param width Border width.
		 */
		void SetBorderWidth( float width );

		/** Get border width.
		 * @return Border width.
		 */
		float GetBorderWidth() const;

		HandleEventResult HandleEvent( const sf::Event& event );

		Signal<void( Widget::Ptr, Widget::Ptr )>  OnAdd; //!< Fired when child added.
		Signal<void( Widget::Ptr, Widget::Ptr )>  OnRemove; //!< Fired when child removed.

	protected:
		/** Constructor.
		 */
		Container();

		/** Get children.
		 * @return std::set with children.
		 */
		WidgetsList& GetChildren();

	private:
		void HandleExpose( Widget::Ptr widget, sf::RenderTarget& target );
		void HandleSizeAllocate( Widget::Ptr widget, const sf::FloatRect& oldallocation );

		float  m_border_width;

		WidgetsList  m_children;
};

}
