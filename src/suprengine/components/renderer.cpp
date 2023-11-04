#include "renderer.h"

#include <suprengine/entity.h>

using namespace suprengine;

Renderer::Renderer( Entity* owner, int priority_order )
	: Component( owner, priority_order )
{
	_render_batch = owner->get_game()->get_render_batch();
}

void Renderer::setup()
{
	_render_batch->add_renderer( this );

}

void Renderer::unsetup()
{
	_render_batch->remove_renderer( this );
}