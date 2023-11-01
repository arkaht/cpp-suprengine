#include "renderer.h"

#include <suprengine/ecs/entity.h>

using namespace suprengine;

Renderer::Renderer( Entity* owner, int priority_order )
	: Component( owner, priority_order )
{
	render_batch = owner->get_game()->get_render_batch();
}

void Renderer::setup()
{
	render_batch->add_renderer( this );

}

void Renderer::unsetup()
{
	render_batch->remove_renderer( this );
}