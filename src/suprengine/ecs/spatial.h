#pragma once
#include "entity.h"

#include "components/transform3.fwd.h"

namespace suprengine
{
	class Spatial : public Entity
	{
	public:
		Transform3* transform { nullptr };

		Spatial();
	};
}