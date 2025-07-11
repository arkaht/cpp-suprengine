#pragma once

#define DEFINE_ENUM_BITFLAG_OPERATOR( Enum, NumType, Op )			\
	inline Enum operator##Op( Enum a, Enum b )						\
	{																\
		NumType result =											\
			static_cast<NumType>( a ) Op static_cast<NumType>( b );	\
		return static_cast<Enum>( result );							\
	}																\
	inline Enum& operator##Op##=( Enum& a, Enum b )					\
	{																\
		a = a Op b;													\
		return a;													\
	}

//  Implement common bitflags operators of the given enum
#define DEFINE_ENUM_WITH_FLAGS( Enum, NumType )				\
	DEFINE_ENUM_BITFLAG_OPERATOR( Enum, NumType, ^ )		\
	DEFINE_ENUM_BITFLAG_OPERATOR( Enum, NumType, & )		\
	DEFINE_ENUM_BITFLAG_OPERATOR( Enum, NumType, | )

namespace suprengine
{
	/*
	 * Check that type has implemented the & operator such as 'T & T' returns a T object.
	 */
	template <typename T>
	concept has_operator_and = requires( const T& lhs, const T& rhs )
	{
		{ lhs & rhs } -> std::same_as<T>;
	};

	/*
	 * Return whenever enum value has the given flag.
	 * Enum must implement the & operator.
	 */
	template <typename Enum>
	std::enable_if_t<has_operator_and<Enum>, bool> enum_has_flag( Enum value, Enum flag )
	{
		return ( value & flag ) == flag;
	}
}