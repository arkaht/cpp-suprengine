#pragma once

#include <string>

//  Use STL size type for rapidjson
#define RAPIDJSON_NO_SIZETYPEDEFINE
namespace rapidjson
{ 
	typedef ::std::size_t SizeType; 
}

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

#include <suprengine/math/color.h>
#include <suprengine/utils/logger.h>

namespace suprengine::json
{
	using document = rapidjson::Document;
	using value = rapidjson::Value;

	/*
	 * Add a JSON value to a JSON document.
	 */
	void add_value( document& doc, const std::string& key, value& value );

	/*
	 * Add a value to a JSON document.
	 */
	template <typename T>
	void add( document& doc, const std::string& key, T value )
	{
		json::value json_value( value );
		add_value( doc, key, json_value );
	}
	template <>
	void add( document& doc, const std::string& key, std::string value );
	template <>
	void add( document& doc, const std::string& key, Color value );

	/*
	 * Return a value from a JSON object.
	 */
	template <typename T>
	T get( const value& obj, const std::string& key, T default_value = T() )
	{
		if ( !obj.HasMember( key.c_str() ) ) 
		{
			Logger::error( "JSON: Couldn't get value of key '%s', return a default value.", key.c_str() );
			return default_value;
		}

		return obj[key.c_str()].Get<T>();
	}
	template <>
	std::string get( const value& obj, const std::string& key, std::string default_value );
	template <>
	Color get( const value& obj, const std::string& key, Color default_value );

	/*
	 * Get a value from a JSON object and fill the given variable with.
	 */
	template <typename T>
	void get( const value& obj, const std::string& key, T* out, T default_value = T() )
	{
		*out = get( obj, key, default_value );
	}
}