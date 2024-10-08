#include "opengl-render-batch.h"

#include <suprengine/assets.h>
#include <suprengine/texture.h>
#include <suprengine/game.h>
#include <suprengine/engine.h>

#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

#include <filesystem>

using namespace suprengine;

constexpr float RECT_VERTICES[] = {
	//  bottom-left
	-1.0f, -1.0f,  //  position
	 0.0f,  0.0f,  //  uv
	//  bottom-right
	1.0f, -1.0f,  //  position
	1.0f,  0.0f,  //  uv
	//  top-right
	1.0f, 1.0f,  //  position
	1.0f, 1.0f,  //  uv
	//  top-left
	-1.0f, 1.0f,  //  position
	0.0f, 1.0f   //  uv
};

constexpr unsigned int RECT_INDICES[] = {
	0, 1, 2,
	2, 3, 0
};

OpenGLRenderBatch::~OpenGLRenderBatch()
{
	ImGui_ImplOpenGL3_Shutdown();

	SDL_GL_DeleteContext( _gl_context );

	_release_framebuffers();

	delete _quad_vertex_array;
	delete _rect_vertex_array;
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

	//  Request a color buffer w/ 8-bits per RGBA channel
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );

	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );  //  depth buffering
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );  //  enable double buffering
	SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 );  //  force OpenGL to use hardware acceleration

	_gl_context = SDL_GL_CreateContext( _window->get_sdl_window() );

	//  GLEW
	glewExperimental = GL_TRUE;
	if ( glewInit() != GLEW_OK )
	{
		Logger::error( "Failed to initialize GLEW." );
		return false;
	}
	Logger::info( "Initialized GLEW" );

	//  Apparently, GLEW emit a beginning error code on some platforms, so we pop it
	glGetError();

	//  Init image library
	if ( IMG_Init( IMG_INIT_PNG ) == 0 )
	{
		Logger::error( "Failed to initialize the SDL Image library" );
		return false;
	}
	Logger::info( "Initialized the SDL Image library" );

	//  Initialize ttf library
	if ( TTF_Init() == -1 )
	{
		Logger::error( "Failed to initialize the SDL TTF library" );
		return false;
	}
	Logger::info( "Initialized the SDL TTF library" );

	//  Enable debug output
	set_debug_output( true );
	glDebugMessageCallback( _message_callback, 0 );

	//  Log OpenGL Version
	auto opengl_version = glGetString( GL_VERSION );
	Logger::info( "OpenGL Version: %s", opengl_version );

	_load_assets();

	return true;
}

bool OpenGLRenderBatch::init_imgui()
{
	auto sdl_window = _window->get_sdl_window();
	if ( !ImGui_ImplSDL2_InitForOpenGL( sdl_window, _gl_context ) )
	{
		Logger::critical( "RenderBatch::OpenGL: failed to initialize ImGui with SDL2!" );
		return false;
	}

	if ( !ImGui_ImplOpenGL3_Init( "#version 130" ) )
	{
		Logger::critical( "RenderBatch::OpenGL: failed to initialize ImGui with OpenGL3!" );
		return false;
	}

	return true;
}

void OpenGLRenderBatch::begin_imgui_frame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}

void OpenGLRenderBatch::begin_render()
{
	//  begin ImGui rendering
	ImGui::Render();

	//  clear screen
	glBindFramebuffer( GL_FRAMEBUFFER, _fbo );
	glClearColor(
		_background_color.r / 255.0f,
		_background_color.g / 255.0f,
		_background_color.b / 255.0f,
		_background_color.a / 255.0f
	);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//  check camera
	auto& engine = Engine::instance();
	_camera = engine.camera;
	if ( _camera == nullptr )
	{
		Logger::error( "no main camera, rendering aborted!" );
		return;
	}

	//  activate shader & vertex array
	_view_matrix = _camera->get_view_matrix() * _camera->get_projection_matrix();
	if ( _color_shader != nullptr )
	{
		_color_shader->activate();
		_color_shader->set_mtx4( "u_view_projection", _view_matrix );
	}
	if ( _texture_shader != nullptr )
	{
		_texture_shader->activate();
		_texture_shader->set_mtx4( "u_view_projection", _view_matrix );
	}
}

void OpenGLRenderBatch::render()
{
	//  draw meshes
	{
		//  enable clockwise
		glFrontFace( GL_CW );

		//  enable face culling
		glEnable( GL_CULL_FACE );	

		//  enable depth testing
		glEnable( GL_DEPTH_TEST );  
		glDepthFunc( GL_LEQUAL );

		//  render
		_render_phase( RenderPhase::World );

		//  disable options
		glDisable( GL_DEPTH_TEST );
		glDisable( GL_CULL_FACE );
	}

	//  draw sprites
	{
		//  setup shaders
		if ( _texture_shader != nullptr )
		{
			_texture_shader->activate();
			_texture_shader->set_mtx4( "u_view_projection", _viewport_matrix );
		}

		//  enable counter-clockwise
		glFrontFace( GL_CCW );

		//  enable blending
		glEnable( GL_BLEND );
		glBlendEquationSeparate( GL_FUNC_ADD, GL_FUNC_ADD );
		glBlendFuncSeparate( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO );
		
		//  render
		_render_phase( RenderPhase::Viewport );

		//  disable options
		glDisable( GL_BLEND );
	}

	//  ImGui rendering
	ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );
}

void OpenGLRenderBatch::end_render()
{
	Vec2 window_size = _window->get_size();
	int width = (int)window_size.x, height = (int)window_size.y;

	//  copy framebuffer into post-process framebuffer
	glBindFramebuffer( GL_READ_FRAMEBUFFER, _fbo );
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, _pp_fbo );
	glBlitFramebuffer( 
		0, 0, width, height, 
		0, 0, width, height, 
		GL_COLOR_BUFFER_BIT, 
		GL_NEAREST 
	);

	//  render framebuffer
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	_rect_vertex_array->activate();
	_framebuffer_shader->activate();
	glBindTexture( GL_TEXTURE_2D, _pp_texture );
	_draw_elements( 6 );
	
	//  populate rendering
	SDL_GL_SwapWindow( _window->get_sdl_window() );
}

void OpenGLRenderBatch::on_window_resized( const Vec2& size )
{
	_viewport_size = size;

	//  update viewport
	glViewport( 0, 0, (int)_viewport_size.x, (int)_viewport_size.y );

	//  update screen offset
	_screen_offset = Vec3( size * 0.5f, 0.0f );

	//  update viewport matrix
	_viewport_matrix = Mtx4::create_simple_view_projection( 
		size.x, size.y
	);

	update_framebuffers();
}

void OpenGLRenderBatch::draw_rect( DrawType draw_type, const Rect& rect, const Color& color )
{
	_quad_vertex_array->activate();
	_color_shader->activate();

	//  setup matrices
	Mtx4 scale_matrix = Mtx4::create_scale( rect.w, rect.h, 1.0f );
	Mtx4 location_matrix = _compute_location_matrix( rect.x, rect.y, 0.0f );
	_color_shader->set_mtx4( "u_world_transform", scale_matrix * location_matrix );
	_color_shader->set_vec4( "u_modulate", color );

	//  draw
	_draw_elements( 6 );
}

void OpenGLRenderBatch::draw_texture( 
	const Rect& src_rect, 
	const Rect& dest_rect, 
	float rotation, 
	const Vec2& origin, 
	Texture* texture, 
	const Color& color
)
{
	//  setup matrices
	Mtx4 scale_matrix = Mtx4::create_scale( 
		dest_rect.w, dest_rect.h, 1.0f );
	Mtx4 rotation_matrix = Mtx4::create_rotation_z( rotation );
	Mtx4 location_matrix = _compute_location_matrix( 
		dest_rect.x, dest_rect.y, 0.0f );

	draw_texture(
		scale_matrix * rotation_matrix * location_matrix, 
		texture, origin, src_rect, color 
	);
}

void OpenGLRenderBatch::draw_texture(
	const Mtx4& matrix, 
	Texture* texture, 
	const Vec2& origin, 
	const Rect& src_rect,
	const Color& color 
)
{
	_quad_vertex_array->activate();
	_texture_shader->activate();
	texture->activate();

	_texture_shader->set_mtx4( "u_world_transform", matrix );

	//  set modulate
	_texture_shader->set_vec4( "u_modulate", color );

	//  source rect
	Vec2 size = texture->get_size();
	_texture_shader->set_vec4( "u_source_rect",
		src_rect.x / size.x,
		src_rect.y / size.y,
		src_rect.w / size.x,
		src_rect.h / size.y
	);
	
	//  origin
	_texture_shader->set_vec2( "u_origin", origin );

	//  draw
	_draw_elements( 6 );
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
		shader->set_vec3( "u_ambient_direction", _ambient_light.direction );
		shader->set_float("u_ambient_scale", _ambient_light.scale );
		shader->set_vec4( "u_ambient_color", _ambient_light.color );
	}

	//  draw
	mesh->get_vertex_array()->activate();
	mesh->get_texture( texture_id )->activate();
	_draw_elements( mesh->get_indices_count() );
}

void OpenGLRenderBatch::draw_model( 
	const Mtx4& matrix, 
	const SharedPtr<Model>& model, 
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
		if ( shader != nullptr )
		{
			shader->activate();
			shader->set_mtx4( "u_view_projection", _view_matrix );  //  TODO: pass this matrix only once
			shader->set_mtx4( "u_world_transform", matrix );
			shader->set_vec4( "u_modulate", color );

			//  lighting
			shader->set_vec3( "u_ambient_direction", _ambient_light.direction );
			shader->set_float( "u_ambient_scale", _ambient_light.scale );
			shader->set_vec4( "u_ambient_color", _ambient_light.color );
		}

		//  draw
		mesh->get_vertex_array()->activate();
		if ( auto texture = mesh->get_texture( 0 ) )
		{
			texture->activate();
		}
		_draw_elements( mesh->get_indices_count() );
	}
}

void OpenGLRenderBatch::draw_debug_model( 
	const Mtx4& matrix, 
	const SharedPtr<Model>& model, 
	const Color& color 
)
{
	if ( model == nullptr ) return;

	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	draw_model(
		matrix,
		model,
		"suprengine::color",
		color
	);
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}

void OpenGLRenderBatch::translate( const Vec2& pos )
{
}

void OpenGLRenderBatch::scale( float zoom )
{}

void OpenGLRenderBatch::clip( const Rect& region )
{
	//  TODO: support clipping
}

void OpenGLRenderBatch::set_debug_output( bool is_active )
{
	if ( is_active )
	{
		glEnable( GL_DEBUG_OUTPUT );
		Logger::info( "OpenGL: Enable debug output" );
	}
	else
	{
		glDisable( GL_DEBUG_OUTPUT );
		Logger::info( "OpenGL: Disable debug output" );
	}
}

void OpenGLRenderBatch::set_samples( unsigned int samples )
{
	_samples = samples;
	Logger::info( "OpenGL: Setting samples to %d", samples );

	update_framebuffers();
}

void OpenGLRenderBatch::update_framebuffers()
{
	Logger::info( "OpenGL: Updating framebuffers" );

	_release_framebuffers();
	_create_framebuffers( (int)_viewport_size.x, (int)_viewport_size.y );
}

void OpenGLRenderBatch::_load_assets()
{
	Logger::info( "Loading engine assets" );

	//  create vertex array
	_quad_vertex_array = new VertexArray(
		VertexArrayPreset::Position3_Normal3_UV2,
		QUAD_VERTICES, 4,
		QUAD_INDICES, 6
	);
	_rect_vertex_array = new VertexArray(
		VertexArrayPreset::Position2_UV2,
		RECT_VERTICES, 4,
		RECT_INDICES, 6
	);

	//  load shaders
	_framebuffer_shader = Assets::load_shader(
		"suprengine::framebuffer",
		"assets/suprengine/shaders/framebuffer.vert",
		"assets/suprengine/shaders/framebuffer.frag"
	);
	_color_shader = Assets::load_shader(
		"suprengine::color",
		"assets/suprengine/shaders/transform.vert",
		"assets/suprengine/shaders/color.frag"
	);
	_texture_shader = Assets::load_shader(
		"suprengine::texture",
		"assets/suprengine/shaders/texture.vert",
		"assets/suprengine/shaders/texture.frag"
	);
	Assets::load_shader(
		SHADER_LIT_MESH,
		"assets/suprengine/shaders/lit-mesh.vert",
		"assets/suprengine/shaders/lit-mesh.frag"
	);

	//  load textures
	Assets::load_texture(
		TEXTURE_LARGE_GRID,
		"assets/suprengine/textures/large-grid.png"
	);
	Assets::load_texture(
		TEXTURE_MEDIUM_GRID,
		"assets/suprengine/textures/medium-grid.png"
	);

	auto texture = Assets::get_texture( TEXTURE_MEDIUM_GRID );

	//  load models
	auto arrow_model = Assets::load_model(
		MESH_ARROW,
		"assets/suprengine/models/arrow.fbx",
		SHADER_LIT_MESH
	);
	arrow_model->get_mesh( 0 )->add_texture( texture );

	auto cube_model = Assets::load_model(
		MESH_CUBE,
		"assets/suprengine/models/cube.fbx",
		SHADER_LIT_MESH
	);
	cube_model->get_mesh( 0 )->add_texture( texture );

	auto cylinder_model = Assets::load_model(
		MESH_CYLINDER,
		"assets/suprengine/models/cylinder.fbx",
		SHADER_LIT_MESH
	);
	cylinder_model->get_mesh( 0 )->add_texture( texture );

	auto sphere_model = Assets::load_model(
		MESH_SPHERE,
		"assets/suprengine/models/sphere.fbx",
		SHADER_LIT_MESH
	);
	sphere_model->get_mesh( 0 )->add_texture( texture );
}

void OpenGLRenderBatch::_create_framebuffers( int width, int height )
{
	//  create framebuffer object
	glGenFramebuffers( 1, &_fbo );
	glBindFramebuffer( GL_FRAMEBUFFER, _fbo );

	//  create framebuffer texture
	int target = _samples > 0 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	glGenTextures( 1, &_framebuffer_texture );
	glBindTexture( target, _framebuffer_texture );
	if ( _samples > 0 )
	{
		glTexImage2DMultisample(
			target, _samples, GL_RGB, width, height, GL_TRUE );
	}
	else
	{
		glTexImage2D(
				target, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );
	}
	glTexParameteri(
		target, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri(
		target, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri(
		target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri(
		target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glFramebufferTexture2D(
		GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0, target,
		_framebuffer_texture,
		0
	);

	//  create renderbuffer object
	glGenRenderbuffers( 1, &_rbo );
	glBindRenderbuffer( GL_RENDERBUFFER, _rbo );
	if ( _samples > 0 )
	{
		glRenderbufferStorageMultisample(
			GL_RENDERBUFFER,
			_samples,
			GL_DEPTH24_STENCIL8,
			width, height
		);
	}
	else
	{
		glRenderbufferStorage(
			GL_RENDERBUFFER,
			GL_DEPTH24_STENCIL8,
			width, height
		);
	}
	glFramebufferRenderbuffer(
		GL_FRAMEBUFFER,
		GL_DEPTH_STENCIL_ATTACHMENT,
		GL_RENDERBUFFER,
		_rbo
	);

	//  error-checking framebuffer
	auto status = glCheckFramebufferStatus( GL_FRAMEBUFFER );
	if ( status != GL_FRAMEBUFFER_COMPLETE )
	{
		Logger::error( "OpenGL: Framebuffer error: " + std::to_string( status ) );
		throw std::exception( "OpenGL Framebuffer couldn't be created!" );
	}
	Logger::info( "OpenGL: Framebuffer created" );

	//  create post-process framebuffer object
	glGenFramebuffers( 1, &_pp_fbo );
	glBindFramebuffer( GL_FRAMEBUFFER, _pp_fbo );

	//  create post-process framebuffer texture
	target = GL_TEXTURE_2D;
	glGenTextures( 1, &_pp_texture );
	glBindTexture( target, _pp_texture );
	glTexImage2D(
		target, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );
	glTexParameteri(
		target, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri(
		target, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri(
		target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri(
		target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glFramebufferTexture2D(
		GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
		_pp_texture,
		0
	);

	//  error-checking post-process framebuffer
	status = glCheckFramebufferStatus( GL_FRAMEBUFFER );
	if ( status != GL_FRAMEBUFFER_COMPLETE )
	{
		Logger::error( "OpenGL: Post-Processing Framebuffer error: " + std::to_string( status ) );
		throw std::exception( "OpenGL Post-Processing Framebuffer couldn't be created!" );
	}
	Logger::info( "OpenGL: Post-Processing Framebuffer created" );
}

void OpenGLRenderBatch::_release_framebuffers()
{
	glDeleteRenderbuffers( 1, &_rbo );
	
	glDeleteTextures( 1, &_framebuffer_texture );
	glDeleteTextures( 1, &_pp_texture );

	glDeleteFramebuffers( 1, &_fbo );
	glDeleteFramebuffers( 1, &_pp_fbo );
}

Mtx4 OpenGLRenderBatch::_compute_location_matrix( float x, float y, float z )
{
	return Mtx4::create_translation(
		Vec3 {
			x - _screen_offset.x,
			y - _screen_offset.y,
			z
		}
	);
}

void OpenGLRenderBatch::_draw_elements( int indices_count )
{
	glDrawElements( GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, nullptr );
}
