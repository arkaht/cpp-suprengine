#pragma once

#include <suprengine/usings.h>

namespace suprengine
{
	template<typename TShareable>
	class Shareable : public std::enable_shared_from_this<TShareable>
	{
	public:
		/*
		 * Dynamic cast to the given class. 
		 * You should check the result before using it.
		 */
		template <typename T>
		shared_ptr<T> cast()
		{
			return std::dynamic_pointer_cast<T>( this->shared_from_this() );
		}
		/*
		 * Static cast to the given class. 
		 * The entity must inherit from the given class.
		 */
		template <typename T>
		shared_ptr<T> as()
		{
			return std::static_pointer_cast<T>( this->shared_from_this() );
		}
	};
}