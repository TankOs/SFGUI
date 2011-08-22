#pragma once
#include <SFGUI/Config.hpp>
#include <SFGUI/Scrollbar.hpp>

namespace sfg {

/** Horizontal Scrollbar widget.
 */
class SFGUI_API HScrollbar : public Scrollbar {
	public:
		typedef boost::shared_ptr<HScrollbar>  Ptr; //!< Shared pointer.

		/** Ctor.
		 */
		HScrollbar();

		/** Dtor.
		 */
		~HScrollbar();
};

}
