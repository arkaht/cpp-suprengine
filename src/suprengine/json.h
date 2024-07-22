#include <string>

//  Use STL size type for rapidjson
#define RAPIDJSON_NO_SIZETYPEDEFINE
namespace rapidjson 
{ 
	typedef ::std::size_t SizeType; 
}

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

#include <suprengine/color.h>

namespace suprengine::json
{
	using document = rapidjson::Document;
	using value = rapidjson::Value;

	void add_string( document& doc, const std::string& key, const std::string& value );
	void add_color( document& doc, const std::string& key, const Color& value );
	void add_int( document& doc, const std::string& key, int value );
	void add_float( document& doc, const std::string& key, float value );
}