#pragma once

#include "ghost.hpp"

class GhostSlowZone : public Entity
{
public:
	GhostSlowZone( Rect zone ) : Entity()
	{
		new RectCollider( this, zone );
		collider->debug_color = { 255, 0, 255, 127 };
	}

	void on_trigger_enter( Collider* other )
	{
		Entity* ent = other->get_owner();

		if ( auto ghost = dynamic_cast<Ghost*>( ent ) )
		{
			//  ignore some states
			switch ( ghost->mover->state )
			{
			case GhostState::FLEE:
			case GhostState::WAIT:
			case GhostState::EATEN:
				return;
			}

			ghost->mover->move_time = ghost->mover->SLOW_MOVE_TIME;
		}
	}

	void on_trigger_exit( Collider* other )
	{
		Entity* ent = other->get_owner();

		if ( auto ghost = dynamic_cast<Ghost*>( ent ) )
		{
			//  ignore some states
			switch ( ghost->mover->state )
			{
			case GhostState::FLEE:
			case GhostState::WAIT:
			case GhostState::EATEN:
				return;
			} 

			ghost->mover->move_time = ghost->mover->NORMAL_MOVE_TIME;
		}
	}
};