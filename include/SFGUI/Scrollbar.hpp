#pragma once
#include <SFGUI/Config.hpp>
#include <SFGUI/Range.hpp>

namespace sfg {

/** Scrollbar base.
 */
class SFGUI_API Scrollbar : public Range {
	public:
		typedef boost::shared_ptr<Scrollbar>  Ptr; //!< Shared pointer.

		/** Ctor.
		 */
		Scrollbar();

		/** Dtor.
		 */
		~Scrollbar();
};

}
