#pragma once

#include <string>
#include <memory>

using rconst_str = const std::string&;

template <class T>
using ref = std::shared_ptr<T>;