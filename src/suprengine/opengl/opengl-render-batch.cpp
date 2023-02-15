#include "opengl-render-batch.h"

#include <suprengine/assets.h>

using namespace suprengine;

OpenGLRenderBatch::~OpenGLRenderBatch()
{
	SDL_GL_DeleteContext( gl_context );
	delete vertex_array;
}

bool OpenGLRenderBatch::initialize()
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

	gl_context = SDL_GL_CreateContext( window->get_sdl_window() );

	//  GLEW
	glewExperimental = GL_TRUE;
	if ( glewInit() != GLEW_OK )
	{
		Logger::error( LOG_CATEGORY::VIDEO, "Failed to initialize GLEW." );
		return false;
	}

	//  apparently, GLEW emit a beginning error code on some platforms, so we pop it
	glGetError();

	//  init SDL_image
	if ( IMG_Init( IMG_INIT_PNG ) == 0 )
	{
		Logger::error( LOG_CATEGORY::VIDEO, "Unable to initialize SDL_image" );
		return false;
	}

	//  create vertex array
	vertex_array = new VertexArray( vertices, 4, indices, 6 );
	shader = Assets::load_shader( "base-unlit",
		"src/suprengine/opengl/shaders/transform.vert",
		"src/suprengine/opengl/shaders/color.frag"
	);

	view_projection = Mtx4::create_simple_view_projection( window->get_width(), window->get_height() );
	screen_offset = Vec3 { -window->get_width() / 2.0f, window->get_height() / 2.0f, 0.0f };

	return true;
}

void OpenGLRenderBatch::begin_render()
{
	//  clear screen
	glClearColor( 
		background_color.r / 255.0f, 
		background_color.g / 255.0f, 
		background_color.b / 255.0f, 
		background_color.a / 255.0f 
	);
	glClear( GL_COLOR_BUFFER_BIT );

	//  enable transparency
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	//  activate shader & vertex array
	shader->activate();
	shader->set_mtx4( "view_projection", view_projection );
	vertex_array->activate();
}

void OpenGLRenderBatch::end_render()
{
	SDL_GL_SwapWindow( window->get_sdl_window() );
}

void OpenGLRenderBatch::draw_rect( DrawType draw_type, const Rect& rect, const Color& color )
{
	//  setup matrices
	Mtx4 scale_matrix = Mtx4::create_scale( rect.w, rect.h, 1.0f );
	Mtx4 location_matrix = compute_location_matrix( Vec3 { rect.x, rect.y, 0.0f } );
	shader->set_mtx4( "world_transform", scale_matrix * location_matrix );
	shader->set_vec4( "modulate", color );

	glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr );
}

void OpenGLRenderBatch::draw_texture( const Rect& src_rect, const Rect& dest_rect, const double rotation, const Vec2& origin, Texture* texture, const Color& color )
{
	//  setup matrices
	Mtx4 scale_matrix = Mtx4::create_scale( dest_rect.w, dest_rect.h, 1.0f );
	Mtx4 location_matrix = compute_location_matrix( Vec3 { dest_rect.x, dest_rect.y, 0.0f } );
	shader->set_mtx4( "world_transform", scale_matrix * location_matrix );
	shader->set_vec4( "modulate", color );
	
	//  draw
	glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr );
}

void OpenGLRenderBatch::scale( float zoom )
{}

void OpenGLRenderBatch::clip( const Rect& region )
{}

SDL_Texture* OpenGLRenderBatch::load_texture_from_surface( SDL_Surface* surface )
{
	return nullptr;
}

Mtx4 OpenGLRenderBatch::compute_location_matrix( const Vec3 pos )
{
	return Mtx4::create_translation(
		Vec3 {
			pos.x + screen_offset.x,
			-pos.y + screen_offset.y,
			pos.z
		}
	);
}
