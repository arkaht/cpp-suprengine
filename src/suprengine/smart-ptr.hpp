#pragma once

#include <suprengine/assert.hpp>
#include <suprengine/usings.h>

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
	 * inheriting std::WeakPtr to handle validity and storage of 
	 * the underlying object.
	 */
	template<typename T>
	class SafePtr : public WeakPtr<T>
	{
	public:
		SafePtr() {}
		SafePtr( WeakPtr<T> ptr )
			: WeakPtr<T>( ptr )
		{}
		SafePtr( SharedPtr<T> ptr )
			: WeakPtr<T>( ptr )
		{}

		/*
		 * Returns whenever the pointer to the object is valid.
		 */
		_NODISCARD bool is_valid() const
		{
			return !WeakPtr<T>::expired();
		}

		_NODISCARD T* operator->()
		{
			ASSERT(is_valid(), "Safe pointer is being used on invalid object.");
			return WeakPtr<T>::get();
		}
	};
}