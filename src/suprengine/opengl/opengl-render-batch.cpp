#include "opengl-render-batch.h"
#include "opengl-texture.hpp"

#include <suprengine/assets.h>
#include <suprengine/game.h>

using namespace suprengine;

OpenGLRenderBatch::~OpenGLRenderBatch()
{
	SDL_GL_DeleteContext( gl_context );
	delete vertex_array;
}

//  https://www.khronos.org/opengl/wiki/OpenGL_Error
void GLAPIENTRY _message_callback( GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam )
{
	fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
		type, severity, message );
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

	//  create vertex array
	vertex_array = new VertexArray( vertices, 4, indices, 6 );
	color_shader = Assets::load_shader( "color",
		"src/suprengine/opengl/shaders/transform.vert",
		"src/suprengine/opengl/shaders/color.frag"
	);
	texture_shader = Assets::load_shader( "texture",
		"src/suprengine/opengl/shaders/texture.vert",
		"src/suprengine/opengl/shaders/texture.frag"
	);

	view_projection = Mtx4::create_simple_view_projection( window->get_width(), -window->get_height() );
	screen_offset = Vec3 { window->get_width() / 2.0f, window->get_height() / 2.0f, 0.0f };

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
	Mtx4 view_matrix = Game::instance().get_camera()->get_matrix() * view_projection;
	color_shader->activate();
	color_shader->set_mtx4( "u_view_projection", view_matrix );
	texture_shader->activate();
	texture_shader->set_mtx4( "u_view_projection", view_matrix );
	vertex_array->activate();
}

void OpenGLRenderBatch::end_render()
{
	SDL_GL_SwapWindow( window->get_sdl_window() );
}

void OpenGLRenderBatch::draw_rect( DrawType draw_type, const Rect& rect, const Color& color )
{
	color_shader->activate();

	//  setup matrices
	Mtx4 scale_matrix = Mtx4::create_scale( rect.w, rect.h, 1.0f );
	Mtx4 location_matrix = compute_location_matrix( rect.x, rect.y, 0.0f );
	color_shader->set_mtx4( "u_world_transform", scale_matrix * location_matrix );
	color_shader->set_vec4( "u_modulate", color );

	//  draw
	glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr );
}

void OpenGLRenderBatch::draw_texture( const Rect& src_rect, const Rect& dest_rect, const double rotation, const Vec2& origin, Texture* texture, const Color& color )
{
	texture_shader->activate();
	texture->activate();

	//  setup matrices
	Mtx4 scale_matrix = Mtx4::create_scale( dest_rect.w, dest_rect.h, 1.0f );
	Mtx4 rotation_matrix = Mtx4::create_rotation_z( -rotation * math::DEG2RAD );
	Mtx4 location_matrix = compute_location_matrix( dest_rect.x, dest_rect.y, 0.0f );
	texture_shader->set_mtx4( "u_world_transform", scale_matrix * rotation_matrix * location_matrix );

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
	glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr );
}

void OpenGLRenderBatch::scale( float zoom )
{}

void OpenGLRenderBatch::clip( const Rect& region )
{
	//  TODO: support clipping
}

Texture* OpenGLRenderBatch::load_texture_from_surface( rconst_str path, SDL_Surface* surface, const TextureParams& params )
{
	return new OpenGLTexture( path, surface, params );
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
