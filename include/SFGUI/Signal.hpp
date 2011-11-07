#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/FastDelegate.hpp>

#include <map>
#include <stdint.h>

namespace sfg {

/** Widget signal.
 * Calls a function if something interesting is happening in a widget. Signals
 * can be connected to multiple endpoints. An endpoint may be a free or member
 * function.
 *
 * For free functions, just pass the function's pointer to Connect(). For
 * member functions (methods) specify the class, method name and object.
 * Examples:\n
 * \code
 * widget->OnClick.Connect( &my_callback ); // Free function binding.
 * widget->OnClick.Connect( &MyClass::MyCallback, this ); // Method binding.
 * \endcode
 *
 */
class SFGUI_API Signal {
	public:
		/** Ctor.
		 */
		Signal();

		/** Connect to free function.
		 * @param delegate Free function.
		 * @return Connection serial, use for disconnecting.
		 */
		uint32_t Connect( Delegate delegate );

		/** Connect to non-static member function.
		 * @param function Function.
		 * @param object Object pointer.
		 * @return Connection serial, use for disconnecting.
		 */
		template <class Class>
		uint32_t Connect( void(Class::*function)(), Class* object );

		/** Emit.
		 */
		void operator()() const;

	private:
		typedef std::map<uint32_t, Delegate> DelegateMap;

		uint32_t m_serial;
		DelegateMap m_delegates;
};

}

#include "Signal.inl"
