#pragma once

#include <memory>

namespace suprengine
{
	/*
	 * Pointer to a non-owned object providing direct yet safe 
	 * access to it. Here, "safe" is described as ensuring the
	 * program would not read or write to a dangling pointer.
	 * 
	 * Safety is your responsiblity: check the validity of the 
	 * object before using it. If the object is invalid, using
	 * the pointer to the object will throw an assertion.
	 * 
	 * Internally using STL's smart pointers and especially 
	 * inheriting std::weak_ptr to handle validity and storage of 
	 * the underlying object.
	 */
	template<typename T>
	class SafePtr : public std::weak_ptr<T>
	{
	public:
		SafePtr() {}
		SafePtr( nullptr_t ) {}
		SafePtr( std::weak_ptr<T> ptr )
			: std::weak_ptr<T>( ptr )
		{}
		SafePtr( std::shared_ptr<T> ptr )
			: std::weak_ptr<T>( ptr )
		{}

		/*
		 * Returns whenever the pointer to the object is valid.
		 */
		[[nodiscard]] bool is_valid() const
		{
			return !std::weak_ptr<T>::expired();
		}
		/*
		 * Returns the raw pointer to the object.
		 */
		[[nodiscard]] T* get() const
		{
			return std::weak_ptr<T>::get();
		}

		[[nodiscard]] T* operator->()
		{
			if ( !is_valid() ) return nullptr;
			return get();
		}
		[[nodiscard]] T* operator->() const
		{
			if ( !is_valid() ) return nullptr;
			return get();
		}
		[[nodiscard]] bool operator==( const SafePtr<T>& ptr ) const
		{
			return get() == ptr.get();
		}

		explicit operator bool() const
		{
			return is_valid();
		}
	};
}