#pragma once
#include "entity.h"

#include "components/transform2.fwd.h"

namespace suprengine
{
	class Entity2D : public Entity
	{
	public:
		Transform2* transform { nullptr };

		Entity2D();
	};
}
