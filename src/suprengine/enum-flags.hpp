#pragma once

#define DEFINE_ENUM_BITFLAG_OPERATOR( Enum, NumType, Op )			\
	inline Enum operator Op( Enum a, Enum b )						\
	{																\
		NumType result =											\
			static_cast<NumType>( a ) Op static_cast<NumType>( b );	\
		return static_cast<Enum>( result );							\
	}

//  Implement common bitflags operators of the given enum
#define DEFINE_ENUM_WITH_FLAGS( Enum, NumType )				\
	DEFINE_ENUM_BITFLAG_OPERATOR( Enum, NumType, ^ )		\
	DEFINE_ENUM_BITFLAG_OPERATOR( Enum, NumType, & )		\
	DEFINE_ENUM_BITFLAG_OPERATOR( Enum, NumType, | )
