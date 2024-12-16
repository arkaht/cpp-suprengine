#pragma once

#include <vector>
#include <functional>
#include <map>

namespace suprengine
{
	template <typename ...TVarargs> 
	class Event
	{
	public:
		using func_signature = std::function<void( TVarargs... )>;

	public:
		void listen( const std::string& key, const func_signature& observer )
		{
			_observers[key] = observer;
		}

		void unlisten( const std::string& key )
		{
			const auto itr = _observers.find( key );
			if ( itr == _observers.end() ) return;

			_observers.erase( itr );  //  TODO: check if erasing during 'invoke()' makes issues  //  yes it does..
		}

		void invoke( TVarargs ...args )
		{
			for ( auto& pair : _observers )
			{
				pair.second( args... );
			}
		}

	private:
		std::map<std::string, func_signature> _observers {};
	};
}