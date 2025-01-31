#include "renderer.h"

#include <suprengine/core/engine.h>

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
	auto& engine = Engine::instance();
	auto render_batch = engine.get_render_batch();
	render_batch->add_renderer( as<Renderer>() );
}

void Renderer::unsetup()
{
	auto& engine = Engine::instance();
	auto render_batch = engine.get_render_batch();
	render_batch->remove_renderer( as<Renderer>() );
}