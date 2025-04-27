#pragma once

#ifdef ENABLE_VISDEBUG
#include <suprengine/math/vec3.h>
#include <suprengine/math/color.h>
#include <suprengine/math/box.h>
#include <suprengine/utils/enum-flags.h>
#include <suprengine/utils/usings.h>

namespace suprengine
{
	enum class DebugChannel : uint8
	{
		None			= 0,

		Entity			= 1 << 0,
		Collider		= 1 << 1,
		Camera			= 1 << 2,

		AI				= 1 << 3,
		Pathfinding		= 1 << 4,
		Lighting		= 1 << 5,
		Particles		= 1 << 6,

		All				= 0xFF,
	};
	DEFINE_ENUM_WITH_FLAGS( DebugChannel, uint8 );

	class VisDebug
	{
	public:
		VisDebug() = delete;

		static void add_box(
			const Vec3& location,
			const Quaternion& rotation,
			const Box& box,
			const Color& color,
			float lifetime = 0.0f,
			DebugChannel channel = DebugChannel::Entity
		);
		static void add_sphere(
			const Vec3& location,
			float radius,
			const Color& color,
			float lifetime = 0.0f,
			DebugChannel channel = DebugChannel::Entity
		);
		static void add_line(
			const Vec3& start,
			const Vec3& end,
			const Color& color,
			float lifetime = 0.0f,
			DebugChannel channel = DebugChannel::Entity
		);

		static void render();

		static bool is_channel_active( DebugChannel channel );

		static size_t get_shapes_count();
		static size_t get_shapes_memory_usage();

	public:
		static DebugChannel active_channels;
	};
}
#endif