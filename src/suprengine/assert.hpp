#pragma once

#include <cassert>

//  Debug only: assert with the specified error message
#define ASSERT(condition, message) assert((message, condition))