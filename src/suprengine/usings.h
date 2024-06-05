#pragma once

#include <string>
#include <memory>

using rconst_str = const std::string&;

template <class T>
using shared_ptr = std::shared_ptr<T>;
template <class T>
using weak_ptr = std::weak_ptr<T>;