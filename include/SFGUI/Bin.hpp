#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Container.hpp>

namespace sfg {

/** Base class for containers with only one child.
 */
class SFGUI_API Bin : public Container {
	public:
		typedef boost::shared_ptr<Bin>  Ptr; //!< Shared pointer.

		/** Get child widget.
		 * @return Child widget.
		 */
		Widget::Ptr GetChild() const;

	protected:
		/** Constructor.
		 */
		Bin();

	private:
		void HandleAdd( Widget::Ptr widget, Widget::Ptr child );

	private:
};

}
