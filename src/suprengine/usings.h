#pragma once

#include <string>
#include <memory>

using rconst_str = const std::string&;

template <class T>
using ref = std::shared_ptr<T>;
template <class T>
using weak_ref = std::weak_ptr<T>;