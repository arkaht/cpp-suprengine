#pragma once

#include <suprengine/safe-ptr.hpp>

#include <string>

using rconst_str = const std::string&;

/*
 * Equivalent to Unreal's FString de-referencing, now you can use:
 * '*str' instead of 'str.c_str()'
 */
static const char* operator*( rconst_str str )
{
	return str.c_str();
}

template <class T>
using SharedPtr = std::shared_ptr<T>;
template <class T>
using WeakPtr = std::weak_ptr<T>;

using uint8  = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;

using int8  = std::int8_t;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;