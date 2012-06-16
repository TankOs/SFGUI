#pragma once

namespace sfg {

SHARED_PTR_INLINE_ATTR ReferenceCountBase::ReferenceCountBase() :
	m_strong_count( 1 ),
	m_weak_count( 1 )
{
}

SHARED_PTR_INLINE_ATTR ReferenceCountBase::~ReferenceCountBase() {
}

SHARED_PTR_INLINE_ATTR void ReferenceCountBase::Destroy() {
	delete this;
}

SHARED_PTR_INLINE_ATTR void ReferenceCountBase::AddStrongReference() {
	++m_strong_count;
}

SHARED_PTR_INLINE_ATTR bool ReferenceCountBase::AddStrongReferenceIfExists() {
	if( !m_strong_count ) {
		return false;
	}

	++m_strong_count;

	return true;
}

SHARED_PTR_INLINE_ATTR void ReferenceCountBase::RemoveStrongReference() {
	if( --m_strong_count == 0 ) {
		Dispose();
		RemoveWeakReference();
	}
}

SHARED_PTR_INLINE_ATTR void ReferenceCountBase::AddWeakReference() {
	++m_weak_count;
}

SHARED_PTR_INLINE_ATTR void ReferenceCountBase::RemoveWeakReference() {
	if( --m_weak_count == 0 ) {
		Destroy();
	}
}

SHARED_PTR_INLINE_ATTR unsigned int ReferenceCountBase::UseCount() const {
	return m_strong_count;
}

template<typename T>
SHARED_PTR_INLINE_ATTR ReferenceCount<T>::ReferenceCount( T* pointer ) :
	m_pointer( pointer )
{
}

template<typename T>
SHARED_PTR_INLINE_ATTR void ReferenceCount<T>::Dispose() {
	delete m_pointer;
}

template<typename T>
SHARED_PTR_INLINE_ATTR StrongReferenceCount::StrongReferenceCount( T* pointer ) :
	m_reference_count( 0 )
{
	m_reference_count = new ReferenceCount<T>( pointer );

	if( !m_reference_count ) {
			delete pointer;
	}
}

SHARED_PTR_INLINE_ATTR StrongReferenceCount::StrongReferenceCount() :
	m_reference_count( 0 )
{
}

SHARED_PTR_INLINE_ATTR StrongReferenceCount::~StrongReferenceCount() {
	if( m_reference_count ) {
		m_reference_count->RemoveStrongReference();
	}
}

SHARED_PTR_INLINE_ATTR StrongReferenceCount::StrongReferenceCount( const StrongReferenceCount& other ) :
	m_reference_count(other.m_reference_count)
{
	if( m_reference_count ) {
		m_reference_count->AddStrongReference();
	}
}

SHARED_PTR_INLINE_ATTR StrongReferenceCount& StrongReferenceCount::operator=( const StrongReferenceCount& other ) {
	ReferenceCountBase* tmp = other.m_reference_count;

	if( tmp != m_reference_count ) {
		if( tmp ) {
			tmp->AddStrongReference();
		}

		if( m_reference_count ) {
			m_reference_count->RemoveStrongReference();
		}

		m_reference_count = tmp;
	}

	return *this;
}

SHARED_PTR_INLINE_ATTR void StrongReferenceCount::Swap(StrongReferenceCount & other) {
	ReferenceCountBase* tmp = other.m_reference_count;
	other.m_reference_count = m_reference_count;
	m_reference_count = tmp;
}

SHARED_PTR_INLINE_ATTR unsigned int StrongReferenceCount::UseCount() const {
	return m_reference_count ? m_reference_count->UseCount() : 0;
}

SHARED_PTR_INLINE_ATTR bool operator==( const StrongReferenceCount& left, const StrongReferenceCount& right ) {
	return left.m_reference_count == right.m_reference_count;
}

SHARED_PTR_INLINE_ATTR bool operator<( const StrongReferenceCount& left, const StrongReferenceCount& right ) {
	return std::less<ReferenceCountBase*>()( left.m_reference_count, right.m_reference_count );
}

SHARED_PTR_INLINE_ATTR WeakReferenceCount::WeakReferenceCount() :
	m_reference_count( 0 )
{
}

SHARED_PTR_INLINE_ATTR WeakReferenceCount::WeakReferenceCount( const StrongReferenceCount& other ) :
	m_reference_count( other.m_reference_count )
{
	if( m_reference_count ) {
		m_reference_count->AddWeakReference();
	}
}

SHARED_PTR_INLINE_ATTR WeakReferenceCount::WeakReferenceCount( const WeakReferenceCount& other ) :
	m_reference_count( other.m_reference_count )
{
	if( m_reference_count ) {
		m_reference_count->AddWeakReference();
	}
}

SHARED_PTR_INLINE_ATTR WeakReferenceCount::~WeakReferenceCount() {
	if( m_reference_count ) {
		m_reference_count->RemoveWeakReference();
	}
}

SHARED_PTR_INLINE_ATTR WeakReferenceCount& WeakReferenceCount::operator=( const StrongReferenceCount& other ) {
	ReferenceCountBase* tmp = other.m_reference_count;

	if( tmp != m_reference_count ) {
		if( tmp ) {
			tmp->AddWeakReference();
		}

		if( m_reference_count ) {
			m_reference_count->RemoveWeakReference();
		}

		m_reference_count = tmp;
	}

	return *this;
}

SHARED_PTR_INLINE_ATTR WeakReferenceCount& WeakReferenceCount::operator= ( const WeakReferenceCount& other ) {
	ReferenceCountBase* tmp = other.m_reference_count;

	if( tmp != m_reference_count ) {
		if( tmp ) {
			tmp->AddWeakReference();
		}

		if( m_reference_count ) {
			m_reference_count->RemoveWeakReference();
		}

		m_reference_count = tmp;
	}

	return *this;
}

SHARED_PTR_INLINE_ATTR void WeakReferenceCount::Swap( WeakReferenceCount& other ) {
	ReferenceCountBase* tmp = other.m_reference_count;
	other.m_reference_count = m_reference_count;
	m_reference_count = tmp;
}

SHARED_PTR_INLINE_ATTR unsigned int WeakReferenceCount::UseCount() const {
	return m_reference_count ? m_reference_count->UseCount() : 0;
}

SHARED_PTR_INLINE_ATTR bool operator==( const WeakReferenceCount& left, const WeakReferenceCount& right ) {
	return left.m_reference_count == right.m_reference_count;
}

SHARED_PTR_INLINE_ATTR bool operator<( const WeakReferenceCount& left, const WeakReferenceCount& right ) {
	return std::less<ReferenceCountBase*>()( left.m_reference_count, right.m_reference_count );
}

SHARED_PTR_INLINE_ATTR StrongReferenceCount::StrongReferenceCount( const WeakReferenceCount& other ) :
	m_reference_count( other.m_reference_count )
{
	if( m_reference_count && !m_reference_count->AddStrongReferenceIfExists() ) {
		//assert( ( "Bad WeakPtr" && 0 ) );
		m_reference_count = 0;
	}
}

template<typename T, typename U, typename V>
SHARED_PTR_INLINE_ATTR void RegisterSharedInstance( const SharedPtr<T>* shared_pointer, const U* pointer, const EnableSharedFromThis<V>* base_pointer ) {
	if( base_pointer ) {
		base_pointer->SetSharedInstance( shared_pointer, const_cast<U*>( pointer ) );
	}
}

SHARED_PTR_INLINE_ATTR void RegisterSharedInstance( ... ) {
}

template<typename T>
SHARED_PTR_INLINE_ATTR SharedPtr<T>::SharedPtr() :
	m_pointer(0),
	m_reference_count()
{
}

template<typename T>
template<typename U>
SHARED_PTR_INLINE_ATTR SharedPtr<T>::SharedPtr( U* pointer ) :
	m_pointer( pointer ),
	m_reference_count( pointer )
{
	RegisterSharedInstance( this, pointer, pointer );
}

template<typename T>
template<typename U>
SHARED_PTR_INLINE_ATTR SharedPtr<T>::SharedPtr( const WeakPtr<U>& other ) :
	m_reference_count( other.m_reference_count )
{
	m_pointer = m_reference_count.UseCount() ? other.m_pointer : 0;
}

template<typename T>
template<typename U>
SHARED_PTR_INLINE_ATTR SharedPtr<T>::SharedPtr( const SharedPtr<U>& other ) :
	m_pointer( other.m_pointer ),
	m_reference_count( other.m_reference_count )
{
}

template<typename T>
template< typename U >
SHARED_PTR_INLINE_ATTR SharedPtr<T>::SharedPtr( const SharedPtr<U>& other, T* pointer ) :
	m_pointer( pointer ),
	m_reference_count( other.m_reference_count )
{
}

template<typename T>
template<typename U>
SharedPtr<T>::SharedPtr( const SharedPtr<U>& other, StaticCastTag ) :
	m_pointer( static_cast<T*>( other.m_pointer ) ),
	m_reference_count( other.m_reference_count )
{
}

template<typename T>
template<typename U>
SHARED_PTR_INLINE_ATTR SharedPtr<T>::SharedPtr( const SharedPtr<U>& other, DynamicCastTag ) :
	m_pointer( dynamic_cast<T*>( other.m_pointer ) ),
	m_reference_count( other.m_reference_count )
{
	if( !m_pointer ) {
		m_reference_count = StrongReferenceCount();
	}
}

template<typename T>
SHARED_PTR_INLINE_ATTR SharedPtr<T>& SharedPtr<T>::operator=( const SharedPtr& other ) {
	SharedPtr<T>( other ).swap( *this );
	return *this;
}

template<typename T>
SHARED_PTR_INLINE_ATTR void SharedPtr<T>::reset() {
	SharedPtr<T>().swap( *this );
}

template<typename T>
template<typename U>
SHARED_PTR_INLINE_ATTR void SharedPtr<T>::reset( U* pointer ) {
	assert( !pointer || ( pointer != m_pointer ) );
	SharedPtr<T>( pointer ).swap( *this );
}

template<typename T>
SHARED_PTR_INLINE_ATTR T& SharedPtr<T>::operator*() const {
	assert( m_pointer );
	return *m_pointer;
}

template<typename T>
SHARED_PTR_INLINE_ATTR T* SharedPtr<T>::operator->() const {
	assert( m_pointer );
	return m_pointer;
}

template<typename T>
SHARED_PTR_INLINE_ATTR T* SharedPtr<T>::get() const {
	return m_pointer;
}

template<typename T>
SHARED_PTR_INLINE_ATTR SharedPtr<T>::operator bool() const {
	return m_pointer != 0;
}

template<typename T>
SHARED_PTR_INLINE_ATTR unsigned int SharedPtr<T>::use_count() const {
	return m_reference_count.UseCount();
}

template<typename T>
SHARED_PTR_INLINE_ATTR void SharedPtr<T>::swap( SharedPtr<T>& other ) {
		T* tmp = other.m_pointer;
		other.m_pointer = m_pointer;
		m_pointer = tmp;

		m_reference_count.Swap( other.m_reference_count );
}

template<typename T, typename U>
SHARED_PTR_INLINE_ATTR bool operator==( const SharedPtr<T>& left, const SharedPtr<U>& right ) {
    return left.get() == right.get();
}

template<typename T, typename U>
SHARED_PTR_INLINE_ATTR bool operator!=( const SharedPtr<T>& left, const SharedPtr<U>& right ) {
	return left.get() != right.get();
}

template<typename T, typename U>
SHARED_PTR_INLINE_ATTR bool operator<( const SharedPtr<T>& left, const SharedPtr<U>& right ) {
	return left.m_reference_count < right.m_reference_count;
}

template<typename T, typename U>
SHARED_PTR_INLINE_ATTR SharedPtr<T> StaticPointerCast( const SharedPtr<U>& shared_pointer ) {
	return SharedPtr<T>( shared_pointer, typename SharedPtr<T>::StaticCastTag() );
}

template<typename T, typename U>
SHARED_PTR_INLINE_ATTR SharedPtr<T> DynamicPointerCast( const SharedPtr<U>& shared_pointer ) {
	return SharedPtr<T>( shared_pointer, typename SharedPtr<T>::DynamicCastTag() );
}

template<typename T>
SHARED_PTR_INLINE_ATTR WeakPtr<T>::WeakPtr() :
	m_pointer( 0 ),
	m_reference_count()
{
}

template<typename T>
template<typename U>
SHARED_PTR_INLINE_ATTR WeakPtr<T>::WeakPtr( const SharedPtr<U>& other ) :
	m_pointer( other.m_pointer ),
	m_reference_count( other.m_reference_count )
{
}

template<typename T>
template<typename U>
SHARED_PTR_INLINE_ATTR WeakPtr<T>::WeakPtr( const WeakPtr<U>& other ) :
	m_pointer( other.lock().get() ),
	m_reference_count( other.m_reference_count )
{
}

template<typename T>
template<typename U>
SHARED_PTR_INLINE_ATTR WeakPtr<T>& WeakPtr<T>::operator=( const SharedPtr<U>& other ) {
	m_pointer = other.m_pointer;
	m_reference_count = other.m_reference_count;
	return *this;
}

template<typename T>
SHARED_PTR_INLINE_ATTR SharedPtr<T> WeakPtr<T>::lock() const {
	return SharedPtr<T>( *this );
}

template<typename T>
SHARED_PTR_INLINE_ATTR unsigned int WeakPtr<T>::use_count() const {
	return m_reference_count.UseCount();
}

template<typename T>
SHARED_PTR_INLINE_ATTR void WeakPtr<T>::reset() {
	WeakPtr<T>().swap(*this);
}

template<typename T>
SHARED_PTR_INLINE_ATTR void WeakPtr<T>::swap( WeakPtr<T>& other ) {
	T* tmp = other.m_pointer;
	other.m_pointer = m_pointer;
	m_pointer = tmp;

	m_reference_count.Swap( other.m_reference_count );
}

template<typename T, typename U>
SHARED_PTR_INLINE_ATTR bool operator<( const WeakPtr<T>& left, const WeakPtr<U>& right ) {
	return left.m_reference_count < right.m_reference_count;
}

template<typename T>
SHARED_PTR_INLINE_ATTR SharedPtr<T> EnableSharedFromThis<T>::shared_from_this() {
	SharedPtr<T> shared_pointer( m_weak_pointer );
	assert( shared_pointer.get() == this );
	return shared_pointer;
}

template<typename T>
SHARED_PTR_INLINE_ATTR SharedPtr<const T> EnableSharedFromThis<T>::shared_from_this() const {
		SharedPtr<const T> shared_pointer( m_weak_pointer );
		assert( shared_pointer.get() == this );
		return shared_pointer;
}

template<typename T>
template<typename U, typename V>
SHARED_PTR_INLINE_ATTR void EnableSharedFromThis<T>::SetSharedInstance( const SharedPtr<U>* shared_pointer, V* pointer ) const {
	if( !m_weak_pointer.use_count() ) {
		m_weak_pointer = SharedPtr<T>( *shared_pointer, pointer );
	}
}

}
