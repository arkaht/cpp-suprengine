#pragma once

#include <vector>
#include <functional>
#include <map>

#include <suprengine/usings.h>

namespace suprengine
{
	/*
	 * 
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
		 * Be aware that it not possible to identify a lambda, therefore un-binding and checking
		 * for listening is not possible, use other methods instead if you need to.
		 * Returns whenever the lambda has been registered.
		 */
		bool listen( const func_signature& callback )
		{
			//	NOTE: This is surely not perfect but it should work for my usage.
			return _add_observer( 0, _observers.size(), callback );
		}
		/*
		 * Bind a free function to the event.
		 * Returns whenever the function has been registered.
		 */
		bool listen( func_type func )
		{
			if ( func == nullptr ) return false;

			const size_t func_address = *reinterpret_cast<size_t*>( &func );
			return _add_observer( 0, func_address, func );
		}
		/*
		 * Bind a class method of a given object to the event.
		 * Returns whenever the method has been registered.
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

			return _add_observer( object_address, func_address, callback );
		}

		bool unlisten( func_type func )
		{
			const size_t func_address = *reinterpret_cast<size_t*>( &func );

			return _remove_observer( 0, func_address );
		}
		template <typename ClassType>
		bool unlisten( class_method<ClassType> func, ClassType* object )
		{
			const size_t object_address = reinterpret_cast<size_t>( object );
			const size_t func_address = *reinterpret_cast<size_t*>( &func );

			return _remove_observer( object_address, func_address );
		}

		void invoke( TVarargs ...args )
		{
			for ( const Observer& observer : _observers )
			{
				observer.callback( args... );
			}
		}

		bool is_listening( func_type func ) const
		{
			if ( func == nullptr ) return false;

			const size_t func_address = *reinterpret_cast<size_t*>( &func );
			return _find_observer( 0, func_address ) != -1;
		}
		template <typename ClassType>
		bool is_listening( class_method<ClassType> func, ClassType* object ) const
		{
			if ( func == nullptr || object == nullptr ) return false;

			const size_t object_address = reinterpret_cast<size_t>( object );
			const size_t func_address = *reinterpret_cast<size_t*>( &func );
			return _find_observer( object_address, func_address ) != -1;
		}

		size_t get_listeners_count() const
		{
			return _observers.size();
		}

	private:
		bool _add_observer(
			const size_t object_address,
			const size_t func_address,
			const func_signature callback
		)
		{
			//	Ensure observer is not already present
			const size_t index = _find_observer( object_address, func_address );
			if ( index != -1 ) return false;

			_observers.emplace_back( object_address, func_address, callback );
			return true;
		}

		bool _remove_observer(
			const size_t object_address,
			const size_t func_address
		)
		{
			//  TODO: check if erasing during 'invoke()' makes issues  //  yes it does..

			const size_t index = _find_observer( object_address, func_address );
			if ( index == -1 ) return false;

			const auto itr = _observers.begin() + index;
			_observers.erase( itr );

			return true;
		}

		size_t _find_observer(
			const size_t object_address,
			const size_t func_address
		) const
		{
			for ( size_t index = 0; index < _observers.size(); index++ )
			{
				const Observer& observer = _observers[index];
				if ( observer.object_address == object_address && observer.func_address == func_address )
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

		struct Observer
		{
			uint64 object_address = 0;
			uint64 func_address = 0;
			func_signature callback {};
		};

	private:
		std::vector<Observer> _observers {};
	};
}