#pragma once

#include <suprengine/utils/safe-ptr.h>

template <class T>
using SharedPtr = std::shared_ptr<T>;
template <class T>
using WeakPtr = std::weak_ptr<T>;