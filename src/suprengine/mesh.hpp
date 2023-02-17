#pragma once

#include <suprengine/opengl/shader.h>

#include <vector>

namespace suprengine
{
	class Mesh
	{
	protected:
		std::vector<Texture*> textures;

	public:
		Mesh() {}
		Mesh( const Mesh& mesh ) = delete;
		void operator=( const Mesh& mesh ) = delete;

		int add_texture( Texture* texture )
		{
			textures.emplace_back( texture );
			return (int) textures.size() - 1;
		}
		Texture* get_texture( int id )
		{
			if ( id < 0 || id >= textures.size() ) return nullptr;
			return textures[id];
		}

		virtual Shader* get_shader() { return nullptr; }
		virtual int get_indices_count() { return 0; }

		virtual void activate( int texture_id = 0 ) {};
	};
}