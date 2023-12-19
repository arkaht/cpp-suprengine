#include "stylized-model-renderer.h"

using namespace puzzle;

StylizedModelRenderer::StylizedModelRenderer( Entity* owner, Model* model, Color modulate, int priority_order )
	: ModelRenderer( owner, model, "stylized", modulate, priority_order ) {}

void StylizedModelRenderer::render()
{
	//  get offset scale
	float offset_scale = 1.0f;
	if ( dynamic_camera_distance_settings.is_active )
	{
		auto engine = owner->get_engine();
		auto camera = engine->camera;

		//  compute distances
		Vec3 dist = transform->location - camera->transform->location;
		float dist_sqr = math::min( 
			dynamic_camera_distance_settings.max_distance_sqr, 
			dist.length_sqr()
		);

		//  add offset scale
		float offset = math::lerp( 
			outline_scale, 
			dynamic_camera_distance_settings.max_outline_scale,
			dist_sqr / dynamic_camera_distance_settings.max_distance_sqr
		);
		offset_scale += offset;
		printf( "%f\n", offset );
	}
	else
	{
		offset_scale += outline_scale;
	}

	//  compute outline matrix
	Mtx4 outline_matrix =
		Mtx4::create_scale( transform->scale * offset_scale )
		* Mtx4::create_from_quaternion( transform->rotation )
		* Mtx4::create_translation( transform->location );

	//  draw outline
	glFrontFace( draw_outline_ccw ? GL_CCW : GL_CW );
	_render_batch->draw_model(
		outline_matrix,
		model,
		shader_name,
		modulate
	);

	//  draw mesh on top of outline
	if ( !draw_only_outline )
	{
		glFrontFace( GL_CW );
		_render_batch->draw_model(
			transform->get_matrix(),
			model,
			shader_name,
			Color::black
		);
	}
}
