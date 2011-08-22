#pragma once
#include <SFGUI/Config.hpp>
#include <SFGUI/Range.hpp>

namespace sfg {

/** Slider widget.
 */
class SFGUI_API Slider : public Range {
	public:
		typedef boost::shared_ptr<Slider>  Ptr; //!< Shared pointer.

		/** Ctor.
		 */
		Slider();

		/** Dtor.
		 */
		~Slider();
};

}
