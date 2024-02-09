#pragma once

#include <components/stylized-model-renderer.h>

#include <suprengine/components/lifetime-component.h>

namespace puzzle
{
	using namespace suprengine;

	class ExplosionEffect : public Entity
	{
	public:
		ExplosionEffect( 
			float explosion_size = 1.0f, 
			Color color = Color::white
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
		//  Lifetime ratio representing maximum explosion scale
		const float FULL_SCALE_AT_LIFETIME_RATIO = 0.35f;

		//  Shrinking speed when lifetime is out
		//  (divided by explosion's size)
		//  (the time it takes delays the entity's death)
		const float LIFETIME_OUT_SHRINK_SPEED = 50.0f;
		//  Speed for lerping outline color (white -> custom color)
		const float COLOR_OUTLINE_SPEED = 8.0f;
		//  Speed for lerping inner color (custom color -> black)
		const float COLOR_INNER_SPEED = 4.0f;

		//  Starting outline scale (progressively shrinking to zero over lifetime)
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
	};
}