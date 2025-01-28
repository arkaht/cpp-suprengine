#pragma once

#include <suprengine/vec3.h>
#include <suprengine/color.h>
#include <suprengine/box.h>
#include <suprengine/enum-flags.hpp>
#include <suprengine/usings.h>

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

		static int get_shapes_count();

	public:
		static DebugChannel active_channels;
	};
}