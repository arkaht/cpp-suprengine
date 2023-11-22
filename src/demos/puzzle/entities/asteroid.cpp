#include "asteroid.h"
#include <suprengine/random.h>

using namespace puzzle;

Asteroid::Asteroid()
{
	_model_renderer = create_component<StylizedModelRenderer>( 
		Assets::get_model( "asteroid" + std::to_string( random::generate( 0, 1 ) ) ),
		Color::from_0x( 0xeb6e3dFF )
	);
}

Asteroid::~Asteroid() {}

void Asteroid::update_this( float dt )
{
	Vec3 movement = linear_direction * dt;
	Vec3 rotation = linear_direction * math::DEG2RAD * dt;

	transform->set_location( transform->location + movement );
	transform->set_rotation( transform->rotation + Quaternion( rotation ) );
}
