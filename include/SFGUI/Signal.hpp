#pragma once
#include <boost/signals2.hpp>
#include <boost/bind.hpp>

namespace sfg {

/** Build signals for widgets.
 * This class is an utility class to make the signal connection process more
 * intuitive. A signal can be connected just by calling Connect() and giving
 * the callback function as a parameter. Any number of slots may be connected.
 *
 * Don't get confused by all the Connect() methods of Signal. Thanks to C++
 * templates, the right one is automatically chosen if you're connecting a
 * signal to a member function.
 */
template <class SigType, class Combiner = boost::signals2::optional_last_value<typename boost::function_traits<SigType>::result_type> >
class Signal {
	public:
		typedef boost::signals2::signal<SigType>  Type; // Signal type.

		/** Connect signal to a slot.
		 * Previously connected slots DO NOT get removed.
		 * @param slot Slot (callback), e.g. &MyCallback.
		 */
		void Connect( const typename Type::slot_type& slot );

		/* Watch out! The following is ugly, but is the only way to make this work
		 * ;-) */

		/** Connect signal to member function, no arguments.
		 * @param memfunc Member function pointer.
		 * @param object Pointer to class instance.
		 */
		template <class RetType, class ClassType, class ObjectType>
		void Connect( RetType (ClassType::*memfunc)(), ObjectType* object );

		/** Connect signal to member function, 1 argument.
		 * @param memfunc Member function pointer.
		 * @param object Pointer to class instance.
		 */
		template <class RetType, class ClassType, class ObjectType, class A1>
		void Connect( RetType (ClassType::*memfunc)( A1 ), ObjectType* object );

		/** Connect signal to member function, 2 arguments.
		 * @param memfunc Member function pointer.
		 * @param object Pointer to class instance.
		 */
		template <class RetType, class ClassType, class ObjectType, class A1, class A2>
		void Connect( RetType (ClassType::*memfunc)( A1, A2 ), ObjectType* object );

		/** Connect signal to member function, 3 arguments.
		 * @param memfunc Member function pointer.
		 * @param object Pointer to class instance.
		 */
		template <class RetType, class ClassType, class ObjectType, class A1, class A2, class A3>
		void Connect( RetType (ClassType::*memfunc)( A1, A2, A3 ), ObjectType* object );

		/** Connect signal to member function, 4 arguments.
		 * @param memfunc Member function pointer.
		 * @param object Pointer to class instance.
		 */
		template <class RetType, class ClassType, class ObjectType, class A1, class A2, class A3, class A4>
		void Connect( RetType (ClassType::*memfunc)( A1, A2, A3, A4 ), ObjectType* object );

		Type  Sig;
};

}

#include "Signal.inl"
