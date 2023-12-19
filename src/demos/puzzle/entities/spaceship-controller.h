#pragma once

#include <suprengine/entity.h>

namespace puzzle
{
	using namespace suprengine;

	class Spaceship;

	struct SpaceshipControlInputs
	{
		float throttle_delta = 0.0f;
		Quaternion desired_rotation = Quaternion::identity;
	};

	class SpaceshipController : public Entity
	{
	public:
		virtual ~SpaceshipController();

		void possess( Spaceship* ship );
		void unpossess();
		Spaceship* get_ship() const { return _possessed_ship; }

		const SpaceshipControlInputs& get_inputs() const { return _inputs; }

	public:
		virtual void on_possess() {};
		virtual void on_unpossess() {};

		virtual void update_inputs( float dt ) = 0;

	public:
		/*
		 * Called when the possessed spaceship has been changed.
		 * 
		 * Params:
		 * - Spaceship* previous
		 * - Spaceship* current
		 */
		Event<Spaceship*, Spaceship*> event_on_possess_changed;

	protected:
		Spaceship* _possessed_ship { nullptr };
		SpaceshipControlInputs _inputs {};

	private:
		bool _suppress_event = false;
	};
}