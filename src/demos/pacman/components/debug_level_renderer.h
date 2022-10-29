#pragma once
#include "../entities/level.fwd.h"

#include <suprengine/ecs/components/renderers/renderer.h>

using namespace suprengine;

class DebugLevelRenderer : public Renderer
{
private:
	Level* level;
public:
	DebugLevelRenderer( Level* owner, Color modulate = Color::white, int priority_order = 0 );

	void render() override;
};