#pragma once

#include <suprengine/usings.h>

namespace suprengine
{
	constexpr float QUAD_VERTICES[] = {
		//  Top-left
		0.0f, 0.0f, 0.0f,  //  Position
		0.0f, 0.0f, 0.0f,  //  Normal
			  0.0f, 0.0f,  //  UV
		//  Top-right
		1.0f, 0.0f, 0.0f,  //  Position
		0.0f, 0.0f, 0.0f,  //  Normal
			  1.0f, 0.0f,  //  UV
		//  Bottom-right
		1.0f, 1.0f, 0.0f,  //  Position
		0.0f, 0.0f, 0.0f,  //  Normal
			  1.0f, 1.0f,  //  UV
		//  Bottom-left
		0.0f, 1.0f, 0.0f,  //  Position
		0.0f, 0.0f, 0.0f,  //  Normal
			  0.0f, 1.0f   //  UV
	};

	constexpr uint32 QUAD_INDICES[] = {
		0, 1, 2,
		2, 3, 0
	};

	/*
	 * A preset which define how a VertexArray should be handled
	 * by OpenGL.
	 */
	class VertexArrayPreset
	{
	public:
		static const VertexArrayPreset Position3;
		static const VertexArrayPreset Position3_Normal3_UV2;
		static const VertexArrayPreset Position2_UV2;

	public:
		constexpr VertexArrayPreset( uint32 positions, uint32 normals, uint32 uvs )
			: positions( positions ), normals( normals ), uvs( uvs )
		{
			stride = positions + normals + uvs;
		}

	public:
		uint32 stride;
		uint32 positions;
		uint32 normals;
		uint32 uvs;
	};

	/*
	 * Class handling buffers creation and binding to an OpenGL vertex array object.
	 * This allows to store vertices and indices of 3D models for OpenGL to render.
	 */
	class VertexArray
	{
	public:
		VertexArray(
			const VertexArrayPreset& preset,
			const float* vertices,
			uint32 vertices_count,
			const uint32* indices,
			uint32 indices_count
		);
		~VertexArray();

		/*
		 * Sets vertex array object to be used for next rendering operations.
		 */
		void activate();

		uint32 get_vertices_count() const;
		uint32 get_indices_count() const;
	
	private:
		uint32 _vao_id = 0;
		uint32 _vbo_id = 0;
		uint32 _ibo_id = 0;

		uint32 _vertices_count = 0;
		uint32 _indices_count = 0;
	};
}