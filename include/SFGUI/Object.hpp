#pragma once

#include <SFGUI/Config.hpp>
#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace sfg {

/** Base class for all Widget, Adjustment and Tooltip objects.
 */
class SFGUI_API Object : public boost::noncopyable {
	public:
		typedef boost::shared_ptr<Object>  Ptr; //!< Shared pointer.

	protected:
		/** Constructor.
		 */
		Object();

	private:
};

}
