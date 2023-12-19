#pragma once

#include <suprengine/components/renderers/model-renderer.hpp>

namespace puzzle
{
	using namespace suprengine;

	struct DynamicCameraDistanceSettings
	{
		bool is_active = false;
		float max_distance_sqr = math::pow( 1024.0f, 2.0f );
		float max_outline_scale = 0.1f;
	};

	class StylizedModelRenderer : public ModelRenderer
	{
	public:
		StylizedModelRenderer(
			Entity* owner,
			Model* model,
			Color modulate = Color::white,
			int priority_order = 0
		);

		void render() override;

	public:
		float outline_scale = 0.025f;
		bool draw_only_outline = false;
		bool draw_outline_ccw = true;

		DynamicCameraDistanceSettings dynamic_camera_distance_settings;
	};
}