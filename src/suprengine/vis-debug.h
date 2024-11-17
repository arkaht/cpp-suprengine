#pragma once

#include <suprengine/vec3.h>
#include <suprengine/color.h>
#include <suprengine/box.h>

#include <vector>

namespace suprengine
{
	class VisDebug
	{
	public:
		VisDebug() = delete;

		static void add_box(
			const Vec3& location,
			const Quaternion& rotation,
			const Box& box,
			const Color& color,
			float lifetime = 0.0f
		);
		static void add_sphere(
			const Vec3& location,
			float radius,
			const Color& color,
			float lifetime = 0.0f
		);
		static void add_line(
			const Vec3& start,
			const Vec3& end,
			const Color& color,
			float lifetime = 0.0f
		);

		static void render();
	};
}