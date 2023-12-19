#pragma once

#include <components/stylized-model-renderer.h>
#include <entities/spaceship-controller.h>
#include <entities/projectile.h>

#include <suprengine/components/colliders/box-collider.h>

namespace puzzle
{
	using namespace suprengine;

	class Spaceship : public Entity
	{
	public:
		Spaceship();
		~Spaceship();

		void update_this( float dt ) override;

		void shoot();

		Vec3 get_shoot_location( const Vec3& axis_scale ) const;
		float get_shoot_time() const { return _shoot_time; }

		float get_throttle() const { return _throttle; }

		void set_color( const Color& color );
		Color get_color() const { return _color; }

	public:
		/*
		 * Called when an entity has been damaged by the spaceship.
		 * 
		 * Parameters:
		 * - Entity* victim
		 */
		Event<Entity*> on_hit;

	public:
		SpaceshipController* controller { nullptr };

	private:
		//  Outline scale on models renderers
		const float MODEL_OUTLINE_SCALE = 0.03f;

		//  Throttle gain per second
		const float THROTTLE_GAIN_SPEED = 0.65f;
		//  Max throttle offset by keeping forward pressed
		const float MAX_THROTTLE_FORWARD_OFFSET = 0.25f;
		/*
		 * Speed at maximum throttle (i.e. 1.0), 
		 * proportionally scaled before and beyond 
		 */
		const float MAX_THROTTLE_SPEED = 90.0f;

		//  Trail starting throttle threshold 
		const float TRAIL_THROTTLE_START = 0.3f;
		//  Trail intensity smooth speed
		const float TRAIL_INTENSITY_SPEED = 2.0f;
		//  Trail model wave animation frequency
		const float TRAIL_WAVE_FREQUENCY = 15.0f;
		//  Trail model wave animation amplitude
		const float TRAIL_WAVE_AMPLITUDE = 0.25f;
		//  Trail model scale offset for intensity factor
		const float TRAIL_MODEL_SCALE_INTENSITY_OFFSET = 0.6f;
		//  Trail model scale
		const Vec3  TRAIL_MODEL_SCALE { 1.0f, 1.0f, 0.5f };
		//  Trail model outline scale added as an offset to scale
		const float TRAIL_MODEL_OUTLINE_SCALE_OFFSET = 2.0f;

		//  Shoot time interval
		const float SHOOT_TIME = 0.15f;

	private:
		void _update_movement( float dt );
		void _update_trail( float dt );

	private:
		float _throttle = 0.0f;
		float _trail_intensity = 0.0f;
		Color _color = Color::green;

		float _shoot_time = 0.0f;

		std::shared_ptr<StylizedModelRenderer> model_renderer;
		std::shared_ptr<StylizedModelRenderer> trail_renderer;
	};
}