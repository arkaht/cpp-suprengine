#pragma once

#include <string>
#include <memory>

using rconst_str = const std::string&;

template <class T>
using SharedPtr = std::shared_ptr<T>;
template <class T>
using WeakPtr = std::weak_ptr<T>;