#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/NonCopyable.hpp>
#include <SFGUI/SharedPtr.hpp>

namespace sfg {

/** Base class for all Widget, Adjustment and Tooltip objects.
 */
class SFGUI_API Object : public NonCopyable {
	public:
		typedef SharedPtr<Object> Ptr; //!< Shared pointer.
		typedef SharedPtr<const Object> PtrConst; //!< Shared pointer.

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
