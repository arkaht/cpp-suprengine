#pragma once

#include <components/stylized-model-renderer.h>

#include <suprengine/curve.h>
#include <suprengine/components/lifetime-component.h>

namespace puzzle
{
	using namespace suprengine;

	class ExplosionEffect : public Entity
	{
	public:
		ExplosionEffect( 
			float explosion_size = 1.0f, 
			Color color = Color::white,
			int model_id = -1
		);

		void update_this( float dt ) override;

	public:
		float explosion_size = 1.0f;
		Color color = Color::white;

	private:
		//  Lifetime base value
		const float LIFETIME = 1.5f;
		//  Lifetime random deviation to add
		const float LIFETIME_DEVIATION = 0.2f;

		//  Maximum outline scale
		const float OUTLINE_SCALE = 0.3f;

		//  Random scale-bounds per axis to apply
		const Vec2 RANDOM_SCALE[3] =
		{
			Vec2 { 0.8f, 1.2f },  //  x-axis
			Vec2 { 0.8f, 1.2f },  //  y-axis
			Vec2 { 0.8f, 1.2f },  //  z-axis
		};

	private:
		float _max_lifetime = 0.0f;
		Vec3 _scale = Vec3::one;

		std::shared_ptr<LifetimeComponent> _lifetime_component;
		std::shared_ptr<StylizedModelRenderer> _model_renderer;

		ref<Curve> _curve_transform_scale;
		ref<Curve> _curve_outline_scale;
		ref<Curve> _curve_outline_color;
		ref<Curve> _curve_inner_color;
	};
}