#include "json.h"

using namespace suprengine;

void json::add_value( 
	json::document& doc, 
	const std::string& key, json::value& json_value 
)
{
	auto& allocator = doc.GetAllocator();
	json::value json_key( key.c_str(), key.size(), allocator );
	doc.AddMember( json_key, json_value, allocator );
}

template<>
void json::add( document& doc, const std::string& key, std::string value )
{
	json::value json_value( value.c_str(), value.size(), doc.GetAllocator() );
	add_value( doc, key, json_value );
}

template<>
void json::add( document& doc, const std::string& key, Color value )
{
	add( doc, key, value.to_hex() );
}

template<>
std::string json::get( 
	const value& obj, 
	const std::string& key, std::string default_value 
)
{
	return get<const char*>( obj, key, default_value.c_str() );
}

template <>
Color json::get<Color>( 
	const value& obj, 
	const std::string& key, Color default_value 
)
{
	return Color::from_hex( json::get<const char*>( obj, key, "00000000" ) );
}
