#pragma once

#include <suprengine/vec3.h>
#include <suprengine/color.h>

#include <vector>

namespace suprengine
{
	struct VisDebugLine
	{
		Vec3 start = Vec3::zero;
		Vec3 end = Vec3::one;

		Color color = Color::white;

		float end_time = 0.0f;
	};

	class VisDebug
	{
	public:
		VisDebug() = delete;

		static void add_line(
			const Vec3& start,
			const Vec3& end,
			const Color& color,
			float lifetime = 0.0f
		);

		static void render();

	private:
		static inline std::vector<VisDebugLine> _lines {};
	};
}