#include "renderer.h"

#include <suprengine/entity.h>

using namespace suprengine;

Renderer::Renderer(  
	Color modulate, 
	int priority_order 
)
	: modulate( modulate ),
	  Component( priority_order )
{}

void Renderer::setup()
{
	auto render_batch = Engine::instance().get_render_batch();
	render_batch->add_renderer( this );
}

void Renderer::unsetup()
{
	auto render_batch = Engine::instance().get_render_batch();
	render_batch->remove_renderer( this );
}