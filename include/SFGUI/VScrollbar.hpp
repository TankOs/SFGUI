#pragma once
#include <SFGUI/Config.hpp>
#include <SFGUI/Scrollbar.hpp>

namespace sfg {

/** Vertical Scrollbar widget.
 */
class SFGUI_API VScrollbar : public Scrollbar {
	public:
		typedef boost::shared_ptr<VScrollbar>  Ptr; //!< Shared pointer.

		/** Ctor.
		 */
		VScrollbar();

		/** Dtor.
		 */
		~VScrollbar();
};

}
