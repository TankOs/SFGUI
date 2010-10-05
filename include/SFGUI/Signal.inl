namespace sfg {

template <class SigType>
void Signal<SigType>::Connect( const typename Type::slot_type& slot ) {
	m_signal.connect( slot );
}

template <class SigType>
template <class RetType, class ClassType, class ObjectType>
void Signal<SigType>::Connect( RetType (ClassType::*memfunc)(), ObjectType* object ) {
	m_signal.connect( boost::bind( memfunc, object ) );
}

template <class SigType>
template <class RetType, class ClassType, class ObjectType, class A1>
void Signal<SigType>::Connect( RetType (ClassType::*memfunc)( A1 ), ObjectType* object ) {
	m_signal.connect( boost::bind( memfunc, object, _1 ) );
}

template <class SigType>
template <class RetType, class ClassType, class ObjectType, class A1, class A2>
void Signal<SigType>::Connect( RetType (ClassType::*memfunc)( A1, A2 ), ObjectType* object ) {
	m_signal.connect( boost::bind( memfunc, object, _1, _2 ) );
}

template <class SigType>
template <class RetType, class ClassType, class ObjectType, class A1, class A2, class A3>
void Signal<SigType>::Connect( RetType (ClassType::*memfunc)( A1, A2, A3 ), ObjectType* object ) {
	m_signal.connect( boost::bind( memfunc, object, _1, _2, _3 ) );
}

}
