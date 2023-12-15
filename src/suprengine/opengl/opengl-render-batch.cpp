#include "opengl-render-batch.h"

#include <suprengine/assets.h>
#include <suprengine/texture.h>
#include <suprengine/game.h>

#include <filesystem>

using namespace suprengine;

OpenGLRenderBatch::~OpenGLRenderBatch()
{
	SDL_GL_DeleteContext( gl_context );
	delete quad_vertex_array;
}

//  https://www.khronos.org/opengl/wiki/OpenGL_Error
void GLAPIENTRY _message_callback( GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam )
{
	fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
		type, severity, message );
}

bool OpenGLRenderBatch::init()
{
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );

	//  request a color buffer w/ 8-bits per RGBA channel
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );

	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );  //  depth buffering
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );  //  enable double buffering
	SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 );  //  force OpenGL to use hardware acceleration

	gl_context = SDL_GL_CreateContext( _window->get_sdl_window() );

	//  GLEW
	glewExperimental = GL_TRUE;
	if ( glewInit() != GLEW_OK )
	{
		Logger::error( "failed to initialize GLEW." );
		return false;
	}

	//  apparently, GLEW emit a beginning error code on some platforms, so we pop it
	glGetError();

	//  init image library
	if ( IMG_Init( IMG_INIT_PNG ) == 0 )
	{
		Logger::error( "failed to initialize image library" );
		return false;
	}

	//  initialize ttf library
	if ( TTF_Init() == -1 )
	{
		Logger::error( "failed to initialize TTF library" );
		return false;
	}

	//  enable debug output
	glEnable( GL_DEBUG_OUTPUT );
	glDebugMessageCallback( _message_callback, 0 );

	//  enable face culling
	glEnable( GL_CULL_FACE );

	//  update viewport on window size change
	_window->on_size_changed.listen( 
		"suprengine::opengl-render-batch", 
		[&]( const Vec2& size ) {
			glViewport( 0, 0, (GLsizei)size.x, (GLsizei)size.y );
		}
	);

	//  create vertex array
	quad_vertex_array = new VertexArray( QUAD_VERTICES, 4, QUAD_INDICES, 6 );

	//  load shaders
	color_shader = Assets::load_shader( "color",
		"assets/suprengine/shaders/transform.vert",
		"assets/suprengine/shaders/color.frag"
	);
	texture_shader = Assets::load_shader( "texture",
		"assets/suprengine/shaders/texture.vert",
		"assets/suprengine/shaders/texture.frag"
	);
	Assets::load_shader( "simple-mesh",
		"assets/suprengine/shaders/simple-mesh.vert",
		"assets/suprengine/shaders/simple-mesh.frag"
	);

	//  load textures
	Assets::load_texture( "suprengine::large-grid", "assets/suprengine/textures/large-grid.png" );
	Assets::load_texture( "suprengine::medium-grid", "assets/suprengine/textures/medium-grid.png" );

	auto texture = Assets::get_texture( "suprengine::medium-grid" );
	
	//  load models
	Model* cube_model = Assets::load_model( 
		"suprengine::cube", 
		"assets/suprengine/models/cube.fbx", 
		"simple-mesh"
	);
	cube_model->get_mesh( 0 )->add_texture( texture );

	Model* cylinder_model = Assets::load_model( 
		"suprengine::cylinder", 
		"assets/suprengine/models/cylinder.fbx", 
		"simple-mesh" 
	);
	cylinder_model->get_mesh( 0 )->add_texture( texture );

	Model* sphere_model = Assets::load_model( 
		"suprengine::sphere", 
		"assets/suprengine/models/sphere.fbx", 
		"simple-mesh" 
	);
	sphere_model->get_mesh( 0 )->add_texture( texture );

	screen_offset = Vec3( _window->get_size() * 0.5f, 0.0f );

	return true;
}

void OpenGLRenderBatch::begin_render()
{
	camera = game->camera;
	if ( camera == nullptr )
	{
		Logger::error( "no main camera, rendering aborted!" );
		return;
	}

	//  clear screen
	glClearColor(
		background_color.r / 255.0f,
		background_color.g / 255.0f,
		background_color.b / 255.0f,
		background_color.a / 255.0f
	);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//  activate shader & vertex array
	_view_matrix = camera->get_view_matrix() * camera->get_projection_matrix();
	if ( color_shader != nullptr )
	{
		color_shader->activate();
		color_shader->set_mtx4( "u_view_projection", _view_matrix );
	}
	if ( texture_shader != nullptr )
	{
		texture_shader->activate();
		texture_shader->set_mtx4( "u_view_projection", _view_matrix );
	}
}

void OpenGLRenderBatch::render()
{
	//  draw meshes
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	render_phase( RenderPhase::MESH );
	glDisable( GL_DEPTH_TEST );

	//  draw sprites
	glEnable( GL_BLEND );
	glBlendEquationSeparate( GL_FUNC_ADD, GL_FUNC_ADD );
	glBlendFuncSeparate( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO );
	render_phase( RenderPhase::SPRITE );
	glDisable( GL_BLEND );
}

void OpenGLRenderBatch::end_render()
{
	SDL_GL_SwapWindow( _window->get_sdl_window() );
}

void OpenGLRenderBatch::draw_rect( DrawType draw_type, const Rect& rect, const Color& color )
{
	quad_vertex_array->activate();
	color_shader->activate();

	//  setup matrices
	Mtx4 scale_matrix = Mtx4::create_scale( rect.w, rect.h, 1.0f );
	Mtx4 location_matrix = compute_location_matrix( rect.x, rect.y, 0.0f );
	color_shader->set_mtx4( "u_world_transform", scale_matrix * location_matrix );
	color_shader->set_vec4( "u_modulate", color );

	//  draw
	draw_elements( 6 );
}

void OpenGLRenderBatch::draw_texture( const Rect& src_rect, const Rect& dest_rect, float rotation, const Vec2& origin, Texture* texture, const Color& color )
{
	//  setup matrices
	Mtx4 scale_matrix = Mtx4::create_scale( dest_rect.w, dest_rect.h, 1.0f );
	Mtx4 rotation_matrix = Mtx4::create_rotation_z( rotation );
	Mtx4 location_matrix = compute_location_matrix( dest_rect.x, dest_rect.y, 0.0f );
	draw_texture( scale_matrix * rotation_matrix * location_matrix, texture, origin, src_rect, color );
}

void OpenGLRenderBatch::draw_texture( const Mtx4& matrix, Texture* texture, const Vec2& origin, const Rect& src_rect, const Color& color )
{
	quad_vertex_array->activate();
	texture_shader->activate();
	texture->activate();

	texture_shader->set_mtx4( "u_world_transform", matrix );

	//  set modulate
	texture_shader->set_vec4( "u_modulate", color );

	//  source rect
	Vec2 size = texture->get_size();
	texture_shader->set_vec4( "u_source_rect",
		src_rect.x / size.x,
		src_rect.y / size.y,
		src_rect.w / size.x,
		src_rect.h / size.y
	);
	
	//  origin
	texture_shader->set_vec2( "u_origin", origin );

	//  draw
	draw_elements( 6 );
}

void OpenGLRenderBatch::draw_mesh( const Mtx4& matrix, Mesh* mesh, int texture_id, const Color& color )
{
	//  set matrices
	Shader* shader = mesh->get_shader();
	shader->activate();
	if ( shader != nullptr )
	{
		shader->set_mtx4( "u_view_projection", _view_matrix );  //  TODO: pass this matrix only once
		shader->set_mtx4( "u_world_transform", matrix );
		shader->set_vec4( "u_modulate", color );

		//  lighting
		shader->set_vec3( "u_ambient_direction", ambient_light.direction );
		shader->set_float("u_ambient_scale", ambient_light.scale );
		shader->set_vec4( "u_ambient_color", ambient_light.color );
	}

	//  draw
	mesh->get_vertex_array()->activate();
	mesh->get_texture( texture_id )->activate();
	draw_elements( mesh->get_indices_count() );
}

void OpenGLRenderBatch::draw_model( 
	const Mtx4& matrix, 
	Model* model, 
	rconst_str shader_name,
	const Color& color
)
{
	if ( model == nullptr ) return;

	for ( int i = 0; i < model->get_mesh_count(); i++ )
	{
		auto mesh = model->get_mesh( i );

		//  get shader
		Shader* shader = nullptr;
		if ( shader_name != "" )
		{
			shader = Assets::get_shader( shader_name );
		}
		else
		{
			shader = mesh->shader_name == ""
				? Assets::get_shader( model->shader_name )
				: mesh->get_shader();
		}

		//  update uniforms
		shader->activate();
		if ( shader != nullptr )
		{
			shader->set_mtx4( "u_view_projection", _view_matrix );  //  TODO: pass this matrix only once
			shader->set_mtx4( "u_world_transform", matrix );
			shader->set_vec4( "u_modulate", color );

			//  lighting
			shader->set_vec3( "u_ambient_direction", ambient_light.direction );
			shader->set_float( "u_ambient_scale", ambient_light.scale );
			shader->set_vec4( "u_ambient_color", ambient_light.color );
		}

		//  draw
		mesh->get_vertex_array()->activate();
		if ( auto texture = mesh->get_texture( 0 ) )
		{
			texture->activate();
		}
		draw_elements( mesh->get_indices_count() );
	}
}

void OpenGLRenderBatch::draw_debug_model( 
	const Mtx4& matrix, 
	Model* model, 
	const Color& color 
)
{
	if ( model == nullptr ) return;

	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	draw_model(
		matrix,
		model,
		"color",
		color
	);
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}

void OpenGLRenderBatch::scale( float zoom )
{}

void OpenGLRenderBatch::clip( const Rect& region )
{
	//  TODO: support clipping
}

Mtx4 OpenGLRenderBatch::compute_location_matrix( float x, float y, float z )
{
	return Mtx4::create_translation(
		Vec3 {
			x - screen_offset.x,
			y - screen_offset.y,
			z
		}
	);
}

void OpenGLRenderBatch::draw_elements( int indices_count )
{
	glDrawElements( GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, nullptr );
}
