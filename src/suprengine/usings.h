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