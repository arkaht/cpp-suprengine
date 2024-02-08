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

	private:
		const float LIFETIME = 1.5f;
		const float LIFETIME_DEVIATION = 0.2f;

		const float FULL_SCALE_AT_LIFETIME_RATIO = 0.35f;

	private:
		float _max_lifetime = 0.0f;

		std::shared_ptr<LifetimeComponent> _lifetime_component;
		std::shared_ptr<StylizedModelRenderer> _model_renderer;
	};
}