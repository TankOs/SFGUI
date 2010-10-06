#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Widget.hpp>
#include <set>

namespace sfg {

/**
 * Base class for container-like widgets.
 */
class SFGUI_API Container : public Widget {
	public:
		typedef boost::shared_ptr<Container>  Ptr; //!< Shared pointer.
		typedef std::set<Widget::Ptr>  WidgetsSet;

		/** Add child.
		 * @param widget Widget to add.
		 */
		void Add( Widget::Ptr widget );

		/** Remove child (from container).
		 * @param widget Widget to remove.
		 */
		void Remove( Widget::Ptr widget );

		/* Check if a widget is a child of this container.
		 * @param widget Widget to search for.
		 */
		bool IsChild( Widget::Ptr widget ) const;

		/** Get children.
		 * @return std::set with children.
		 */
		const WidgetsSet& GetChildren() const;

	protected:
		/** Constructor.
		 */
		Container();

		/** Get children.
		 * @return std::set with children.
		 */
		WidgetsSet& GetChildren();

	private:

		WidgetsSet  m_children;
};

}
