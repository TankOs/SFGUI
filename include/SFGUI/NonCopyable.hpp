#pragma once

#include <SFGUI/Config.hpp>

namespace sfg {

/** NonCopyable interface.
 * Prevents derivates from being copied/assigned.
 */
class SFGUI_API NonCopyable {
	protected:
		NonCopyable();
		~NonCopyable();

	private:
		NonCopyable( const NonCopyable& );
		const NonCopyable& operator=( const NonCopyable& );
};

}
