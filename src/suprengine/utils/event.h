#pragma once

#include <vector>
#include <functional>

#include "usings.h"

namespace suprengine
{
	/*
	 * Templated class representing an event able to call all its bound listeners
	 * with user-defined parameters. An implementation of the "Observer" programming
	 * pattern focused on binding functions and methods rather than using interfaces.
	 * 
	 * It allows to, first, elegantly know at compile-time the event you're listening
	 * to without having to cast or having to maintain an enum as your amount of events
	 * grows and, second, to have as many events as you want on one object and have fine
	 * control over the parameters of every single event.
	 * 
	 * Binding lambdas is supported but not un-binding or identifying.
	 * Doesn't support un-binding during the broadcast.
	 * 
	 * If this implementation is too much template-heavy for you, you can look for the simpler one:
	 * https://github.com/arkaht/cpp-suprengine/blob/a5a0d2190df31290c18838820d1cd1513c4cdc2d/src/suprengine/event.h
	 */
	template <typename ...TVarargs>
	class Event
	{
	public:
		using func_type = void( TVarargs... );
		using func_signature = std::function<func_type>;

		template <typename ClassType>
		using class_method = void( ClassType::* )( TVarargs... );

	public:
		/*
		 * Bind a lambda function to the event.
		 * It is currently not possible to identify a lambda, therefore un-binding and checking
		 * for listening is not possible, use other methods instead if you need to.
		 * Returns whenever the lambda has been bound.
		 */
		bool listen( const func_signature& callback )
		{
			//	NOTE: This is surely not perfect but it should work for my usage.
			//	Concerning un-binding lambdas, I could theoritically return the index of where
			//	the callback is put, however as this is already a weak support for lambdas,
			//	I don't want to further encourage using them.
			return _add_listener( 0, ++next_lambda_id, callback );
		}
		/*
		 * Bind a free function to the event.
		 * Trying to bind the same function will fail to add the callback and return false.
		 * Returns whenever the function has been bound.
		 */
		bool listen( func_type func )
		{
			if ( func == nullptr ) return false;

			const size_t func_address = *reinterpret_cast<size_t*>( &func );
			return _add_listener( 0, func_address, func );
		}
		/*
		 * Bind a class method of a given object to the event.
		 * Trying to bind the same method with the same object will fail to add the callback
		 * and return false.
		 * Returns whenever the method has been bound.
		 */
		template <typename ClassType>
		bool listen( class_method<ClassType> func, ClassType* object )
		{
			if ( func == nullptr || object == nullptr ) return false;

			const size_t object_address = reinterpret_cast<size_t>( object );
			const size_t func_address = *reinterpret_cast<size_t*>( &func );
			const func_signature callback = _create_callback(
				func, object,
				std::index_sequence_for<TVarargs...> {}
			);

			return _add_listener( object_address, func_address, callback );
		}

		/*
		 * Un-bind a free function from the event.
		 * Returns whenever the function has been un-bound.
		 */
		bool unlisten( func_type func )
		{
			const size_t func_address = *reinterpret_cast<size_t*>( &func );

			return _remove_listener( 0, func_address );
		}
		/*
		 * Un-bind a class method of a given object from the event.
		 * Returns whenever the function has been un-bound.
		 */
		template <typename ClassType>
		bool unlisten( class_method<ClassType> func, ClassType* object )
		{
			const size_t object_address = reinterpret_cast<size_t>( object );
			const size_t func_address = *reinterpret_cast<size_t*>( &func );

			return _remove_listener( object_address, func_address );
		}

		/*
		 * Broadcast all listeners' callback with given parameters.
		 * Un-binding a listener during the call is not supported and will lead
		 * to iterator invalidation.
		 */
		void invoke( TVarargs ...args )
		{
			for ( const Listener& listener : _listeners )
			{
				listener.callback( args... );
			}
		}

		/*
		 * Returns whenever a free function is listening to the event.
		 */
		bool is_listening( func_type func ) const
		{
			if ( func == nullptr ) return false;

			const size_t func_address = *reinterpret_cast<size_t*>( &func );
			return _find_listener( 0, func_address ) != -1;
		}
		/*
		 * Returns whenever a class method is listening to the event. 
		 */
		template <typename ClassType>
		bool is_listening( class_method<ClassType> func, ClassType* object ) const
		{
			if ( func == nullptr || object == nullptr ) return false;

			const size_t object_address = reinterpret_cast<size_t>( object );
			const size_t func_address = *reinterpret_cast<size_t*>( &func );
			return _find_listener( object_address, func_address ) != -1;
		}

		/*
		 * Returns the number of listeners currently bound to the event.
		 */
		size_t get_listeners_count() const
		{
			return _listeners.size();
		}

		/*
		 * Returns estimated memory usage of the internal listeners vector.
		 */
		size_t get_listeners_memory_usage() const
		{
			return sizeof( Listener ) * get_listeners_capacity();
		}

		/*
		 * Returns current internal listeners vector capacity.
		 */
		size_t get_listeners_capacity() const
		{
			return _listeners.capacity();
		}

	private:
		bool _add_listener(
			const size_t object_address,
			const size_t func_address,
			const func_signature callback
		)
		{
			//	Ensure listener is not already present
			const size_t index = _find_listener( object_address, func_address );
			if ( index != -1 ) return false;

			_listeners.emplace_back( object_address, func_address, callback );
			return true;
		}

		bool _remove_listener(
			const size_t object_address,
			const size_t func_address
		)
		{
			//  TODO: Find a way to support removing listeners during an invoke() call
			//	without adding too much on the memory usage.
			const size_t index = _find_listener( object_address, func_address );
			if ( index == -1 ) return false;

			const auto itr = _listeners.begin() + index;
			_listeners.erase( itr );

			return true;
		}

		size_t _find_listener(
			const size_t object_address,
			const size_t func_address
		) const
		{
			for ( size_t index = 0; index < _listeners.size(); index++ )
			{
				const Listener& listener = _listeners[index];
				if ( listener.object_address == object_address && listener.func_address == func_address )
				{
					return index;
				}
			}

			return -1;
		}

		template <typename ClassType, std::size_t... Indices>
		func_signature _create_callback(
			class_method<ClassType> func, ClassType* object,
			std::index_sequence<Indices...>
		)
		{
			return std::bind( func, object, std::_Ph<Indices + 1> {}... );
		}

		struct Listener
		{
			size_t object_address = 0;
			size_t func_address = 0;
			func_signature callback {};
		};

	private:
		inline static size_t next_lambda_id = 0;

	private:
		std::vector<Listener> _listeners {};
	};
}