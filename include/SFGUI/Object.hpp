#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Signal.hpp>
#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace sfg {

/** Base class for all Widget, Adjustment and Tooltip objects.
 */
class SFGUI_API Object : public boost::noncopyable {
	public:
		typedef boost::shared_ptr<Object>  Ptr; //!< Shared pointer.
		typedef boost::shared_ptr<const Object>  PtrConst; //!< Shared pointer.

		/** Dtor.
		 */
		virtual ~Object();

	protected:
		/** Constructor.
		 */
		Object();

	private:
};

}
