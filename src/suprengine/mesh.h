#pragma once

#include <suprengine/opengl/shader.h>
#include <suprengine/texture.fwd.h>

#include <suprengine/usings.h>

#include <vector>

namespace suprengine
{
	class Mesh
	{
	protected:
		std::vector<Texture*> _textures;

	public:
		std::string shader_name { "simple-mesh" };

		Mesh() {}
		Mesh( rconst_str shader_name ) : shader_name( shader_name ) {}
		Mesh( const Mesh& mesh ) = delete;
		void operator=( const Mesh& mesh ) = delete;

		int add_texture( Texture* texture )
		{
			_textures.emplace_back( texture );
			return (int) _textures.size() - 1;
		}
		Texture* get_texture( int id )
		{
			if ( id < 0 || id >= _textures.size() ) return nullptr;
			return _textures[id];
		}

		virtual Shader* get_shader();
		virtual int get_indices_count() { return 0; }

		virtual void activate( int texture_id = 0 ) {};
	};
}