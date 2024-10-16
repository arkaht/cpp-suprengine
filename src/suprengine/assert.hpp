#pragma once

#include <suprengine/logger.h>

/*
 * Assert a condition.
 * If failed, log the given error message and exits the program.
 */
#define ASSERT( condition, message )						\
	if ( !( condition ) ) {									\
		Logger::critical(									\
			"Assertion failed: %s: \"%s\" [%s:%d]",			\
			#condition,										\
			message,										\
			__FILE__,										\
			__LINE__										\
		);													\
		__debugbreak();										\
	}