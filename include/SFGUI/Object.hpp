#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/NonCopyable.hpp>
#include <memory>

namespace sfg {

/** Base class for all Widget, Adjustment and Tooltip objects.
 */
class SFGUI_API Object : public NonCopyable {
	public:
		typedef std::shared_ptr<Object> Ptr; //!< Shared pointer.
		typedef std::shared_ptr<const Object> PtrConst; //!< Shared pointer.

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
