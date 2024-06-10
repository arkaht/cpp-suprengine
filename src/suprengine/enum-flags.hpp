#pragma once

#define DEFINE_ENUM_BITFLAG_OPERATOR( Enum, NumType, Op )	\
	inline Enum operator Op(Enum a, Enum b)					\
	{														\
		return (Enum)((NumType)a Op (NumType)b);			\
	}

//  Implement common bitflags operators of the given enum
#define DEFINE_ENUM_WITH_FLAGS( Enum, NumType )				\
	DEFINE_ENUM_BITFLAG_OPERATOR( Enum, NumType, ^ )		\
	DEFINE_ENUM_BITFLAG_OPERATOR( Enum, NumType, & )		\
	DEFINE_ENUM_BITFLAG_OPERATOR( Enum, NumType, | )
