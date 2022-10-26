#include "renderer.h"
#include "../entity.h"

using namespace suprengine;

Renderer::Renderer( Entity* owner, Color modulate, int priority_order )
	: modulate( modulate ), Component( owner, priority_order )
{
	render_batch = owner->get_game()->get_render_batch();
	render_batch->add_renderer( this );
}

Renderer::~Renderer()
{
	render_batch->remove_renderer( this );
}
