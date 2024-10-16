#include "model-renderer.hpp"

#include <gl/glew.h>

using namespace suprengine;

void ModelRenderer::render( RenderBatch* render_batch )
{
	glFrontFace( GL_CW );
	render_batch->draw_model( 
		transform->get_matrix(), 
		model, 
		shader_name,
		modulate
	);
}