#include "json.h"

using namespace suprengine;

static void _add_value( 
	json::document& doc, 
	const std::string& key, json::value&& json_value 
)
{
	auto& allocator = doc.GetAllocator();
	json::value json_key( key.c_str(), key.size(), allocator );
	doc.AddMember( json_key, json_value, allocator );
}

//  NOTE: When creating string values whose lifetime end before 
//		  document writing, use GenericDocument::GetAllocator()
//		  to copy the value. Otherwise, it will reference a null value.

void json::add_string( 
	json::document& doc, 
	const std::string& key, const std::string& value 
)
{
	json::value json_value( value.c_str(), value.size(), doc.GetAllocator() );
	_add_value( doc, key, std::move( json_value ) );
}

void json::add_color( 
	document& doc, 
	const std::string& key, const Color& value
)
{
	std::string hex = value.to_hex();
	json::value json_value( hex.c_str(), hex.size(), doc.GetAllocator() );
	_add_value( doc, key, std::move( json_value ) );
}

void json::add_int( 
	document& doc, 
	const std::string& key, int value 
)
{
	_add_value( doc, key, json::value( value ) );
}

void json::add_float( 
	document& doc, 
	const std::string& key, float value 
)
{
	_add_value( doc, key, json::value( value ) );
}
