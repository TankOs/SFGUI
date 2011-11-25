#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/FastDelegate.hpp>

#include <vector>

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
		unsigned int Connect( Delegate delegate );

		/** Connect to non-static member function.
		 * @param function Function.
		 * @param object Object pointer.
		 * @return Connection serial, use for disconnecting.
		 */
		template <class Class>
		unsigned int Connect( void(Class::*function)(), Class* object );

		/** Emit.
		 */
		void operator()() const;

	private:
		struct DelegatePair {
			unsigned int serial;
			Delegate delegate;
		};

		typedef std::vector<DelegatePair> DelegateMap;

		unsigned int m_serial;
		DelegateMap m_delegates;
		std::size_t m_delegates_count;
};

}

#include "Signal.inl"
