#pragma once

#include <suprengine/logger.h>

/*
 * Assert a condition.
 * If failed, logs and debug breaks the program.
 */
//	TODO: Replace __debugbreak
#define ASSERT( condition )									\
	if ( !( condition ) ) {									\
		Logger::critical(									\
			"Assertion failed: %s: [%s:%d]",				\
			#condition,										\
			__FILE__,										\
			__LINE__										\
		);													\
		__debugbreak();										\
	}

/*
 * Assert a condition.
 * If failed, logs the given error message and debug breaks the program.
 */
//	TODO: Replace __debugbreak
#define ASSERT_MSG( condition, message )					\
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