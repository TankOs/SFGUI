#pragma once

#include <SFGUI/NonCopyable.hpp>

#include <cassert>
#include <functional>

///////////////////////////////////////
// WARNING:
// Define only for profiling purposes.
// This WILL slow the library down A LOT.
//#define SHARED_PTR_PROFILE
///////////////////////////////////////

#if defined( __GNUC__ )

#if defined( SHARED_PTR_PROFILE )
#define SHARED_PTR_INLINE_ATTR __attribute__((noinline)) inline
#else
#define SHARED_PTR_INLINE_ATTR __attribute__((always_inline)) inline
#endif

#elif defined( _MSC_VER )

#if defined( SHARED_PTR_PROFILE )
#define SHARED_PTR_INLINE_ATTR __declspec(noinline) inline
#else
#define SHARED_PTR_INLINE_ATTR __forceinline
#endif

#else

#define SHARED_PTR_INLINE_ATTR inline

#endif

namespace sfg {

class ReferenceCountBase;

template<typename T>
class ReferenceCount;

class StrongReferenceCount;
class WeakReferenceCount;

template<typename T>
class SharedPtr;

template<typename T>
class WeakPtr;

template<typename T>
class EnableSharedFromThis;

/// @cond
class ReferenceCountBase : public NonCopyable {
	public:
		ReferenceCountBase();

		virtual ~ReferenceCountBase();

		virtual void Dispose() = 0;

		virtual void Destroy();

		void AddStrongReference();

		bool AddStrongReferenceIfExists();

		void RemoveStrongReference();

		void AddWeakReference();

		void RemoveWeakReference();

		unsigned int UseCount() const;

	private:
		unsigned int m_strong_count;
		unsigned int m_weak_count;
};

template<typename T>
class ReferenceCount : public ReferenceCountBase {
	public:
		explicit ReferenceCount( T* pointer );

		virtual void Dispose();

	private:
		T* m_pointer;
};

class StrongReferenceCount {
	public:
		StrongReferenceCount();

		template<typename T>
		explicit StrongReferenceCount( T* pointer );

		~StrongReferenceCount();

		StrongReferenceCount( const StrongReferenceCount& other );

		explicit StrongReferenceCount( const WeakReferenceCount& other );

		StrongReferenceCount& operator=( const StrongReferenceCount& other );

		void Swap(StrongReferenceCount & other);

		unsigned int UseCount() const;

		friend SHARED_PTR_INLINE_ATTR bool operator==( const StrongReferenceCount& left, const StrongReferenceCount& right );

		friend SHARED_PTR_INLINE_ATTR bool operator<( const StrongReferenceCount& left, const StrongReferenceCount& right );

	private:
		ReferenceCountBase* m_reference_count;

		friend class WeakReferenceCount;
};

class WeakReferenceCount {
	public:
		WeakReferenceCount();

		WeakReferenceCount( const StrongReferenceCount& other );

		WeakReferenceCount( const WeakReferenceCount& other );

		~WeakReferenceCount();

		WeakReferenceCount& operator=( const StrongReferenceCount& other );

		WeakReferenceCount& operator= ( const WeakReferenceCount& other );

		void Swap( WeakReferenceCount& other );

		unsigned int UseCount() const;

		friend SHARED_PTR_INLINE_ATTR bool operator==( const WeakReferenceCount& left, const WeakReferenceCount& right );

		friend SHARED_PTR_INLINE_ATTR bool operator<( const WeakReferenceCount& left, const WeakReferenceCount& right );

	private:
		ReferenceCountBase* m_reference_count;

		friend class StrongReferenceCount;
};
/// @endcond

template<typename T, typename U, typename V>
SHARED_PTR_INLINE_ATTR void RegisterSharedInstance( const SharedPtr<T>* shared_pointer, const U* pointer, const EnableSharedFromThis<V>* base_pointer );

SHARED_PTR_INLINE_ATTR void RegisterSharedInstance( ... );

/** SFGUI Shared Pointer
 */
template<typename T>
class SharedPtr {
	public:
		/// @cond
		struct StaticCastTag {};
		struct DynamicCastTag {};
		/// @endcond

		SharedPtr();

		template<typename U>
		explicit SharedPtr( U* pointer );

		template<typename U>
		explicit SharedPtr( const WeakPtr<U>& other);

		template<typename U>
		SharedPtr( const SharedPtr<U>& other );

		template< typename U >
		SharedPtr( const SharedPtr<U>& other, T* pointer );

		template<typename U>
		SharedPtr( const SharedPtr<U>& other, StaticCastTag );

		template<typename U>
		SharedPtr( const SharedPtr<U>& other, DynamicCastTag );

		SharedPtr& operator=( const SharedPtr& other );

		void reset();

		template<typename U>
		void reset( U* pointer );

		T& operator*() const;

		T* operator->() const;

		T* get() const;

		operator bool() const;

		unsigned int use_count() const;

		void swap( SharedPtr<T>& other );

		template<typename U, typename V>
		friend SHARED_PTR_INLINE_ATTR bool operator<( const SharedPtr<U>& left, const SharedPtr<V>& right );

	private:
		template<typename U>
		friend class SharedPtr;

		template<typename U>
		friend class WeakPtr;

		T* m_pointer;
		StrongReferenceCount m_reference_count;
};

template<typename T, typename U>
SHARED_PTR_INLINE_ATTR bool operator==( const SharedPtr<T>& left, const SharedPtr<U>& right );

template<typename T, typename U>
SHARED_PTR_INLINE_ATTR bool operator!=( const SharedPtr<T>& left, const SharedPtr<U>& right );

template<typename T, typename U>
SHARED_PTR_INLINE_ATTR bool operator<( const SharedPtr<T>& left, const SharedPtr<U>& right );

template<typename T, typename U>
SharedPtr<T> StaticPointerCast( const SharedPtr<U>& shared_pointer );

template<typename T, typename U>
SharedPtr<T> DynamicPointerCast( const SharedPtr<U>& shared_pointer );

/** SFGUI Weak Pointer
 */
template<typename T>
class WeakPtr
{
	public:
		WeakPtr();

		template<typename U>
		WeakPtr( const SharedPtr<U>& other );

		template<typename U>
		WeakPtr( const WeakPtr<U>& other );

		template<typename U>
		WeakPtr& operator=( const SharedPtr<U>& other );

		SharedPtr<T> lock() const;

		unsigned int use_count() const;

		void reset();

		void swap( WeakPtr<T>& other );

		template<typename U, typename V>
		friend SHARED_PTR_INLINE_ATTR bool operator<( const WeakPtr<U>& left, const WeakPtr<V>& right );

	private:
		template<typename U>
		friend class WeakPtr;

		template<typename U>
		friend class SharedPtr;

		T* m_pointer;
		WeakReferenceCount m_reference_count;
};

/** Interface to enable a class to access it's own shared instance.
 */
template<typename T>
class EnableSharedFromThis : public NonCopyable
{
	public:
		SharedPtr<T> shared_from_this();

		SharedPtr<const T> shared_from_this() const;

		template<typename U, typename V>
		void SetSharedInstance( const SharedPtr<U>* shared_pointer, V* pointer ) const;

	private:
		mutable WeakPtr<T> m_weak_pointer;
};

}

#include "SharedPtr.inl"
