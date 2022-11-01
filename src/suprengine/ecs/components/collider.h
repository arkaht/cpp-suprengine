#pragma once
#include "collider.fwd.h"

#include "../component.h"

#include <unordered_set>

namespace suprengine
{
	class Collider : public Component
	{
	protected:
		std::unordered_set<Collider*> collisions;

		const Color COLOR { 0, 255, 0, 127 };
	public:
		Collider( Entity* owner, int priority_order = 0 );
		~Collider() override;

		virtual bool intersects( Collider* other ) { return false; }

		void update_collision_with( Collider* other, bool active );
	};
}