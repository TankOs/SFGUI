#pragma once
#include <SFGUI/Config.hpp>
#include <SFGUI/Widget.hpp>

namespace sfg {

/** Range widget, base class for scrollbars and sliders.
 */
class SFGUI_API Range : public Widget {
	public:
		typedef boost::shared_ptr<Range>  Ptr; //!< Shared pointer.

		/** Ctor.
		 */
		Range();

		/** Dtor.
		 */
		~Range();
};

}
