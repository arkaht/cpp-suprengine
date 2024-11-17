#include "vis-debug.h"

#include <suprengine/engine.h>
#include <suprengine/assets.h>

using namespace suprengine;


class VisDebugShape
{
public:
	Color color = Color::white;

	float end_time = 0.0f;

public:
	virtual void render( RenderBatch* render_batch ) = 0;

	void set_lifetime( float lifetime )
	{
		Updater* updater = Engine::instance().get_updater();
		end_time = updater->get_accumulated_seconds() + lifetime;
	}
};

struct VisDebugBox : public VisDebugShape
{
	Vec3 location {};
	Quaternion rotation = Quaternion::identity;
	Box box {};

	virtual void render( RenderBatch* render_batch ) override
	{
		const auto model = Assets::get_model( MESH_CUBE );
		const Mtx4 matrix = Mtx4::create_from_transform(
			box.get_size(),
			rotation,
			location + box.get_center()
		);

		render_batch->draw_debug_model( matrix, model, color );
	}
};

struct VisDebugSphere : public VisDebugShape
{
	Vec3 location {};
	float radius = 0.0f;

	virtual void render( RenderBatch* render_batch ) override
	{
		const auto model = Assets::get_model( MESH_SPHERE );
		const Mtx4 matrix = Mtx4::create_from_transform(
			Vec3( radius ),
			Quaternion::identity,
			location
		);

		render_batch->draw_debug_model( matrix, model, color );
	}
};

struct VisDebugLine : public VisDebugShape
{
	Vec3 start = Vec3::zero;
	Vec3 end = Vec3::one;

	virtual void render( RenderBatch* render_batch ) override
	{
		render_batch->draw_line( start, end, color );
	}
};


static std::vector<std::unique_ptr<VisDebugShape>> shapes {};

template <typename T>
static void push_shape( const T& shape )
{
	shapes.emplace_back( std::make_unique<T>( shape ) );
}


void VisDebug::add_box(
	const Vec3& location,
	const Quaternion& rotation,
	const Box& box,
	const Color& color,
	float lifetime
)
{
	VisDebugBox shape {};
	shape.location = location;
	shape.rotation = rotation;
	shape.box = box;
	shape.color = color;
	shape.set_lifetime( lifetime );

	push_shape( shape );
}

void VisDebug::add_sphere(
	const Vec3& location,
	float radius,
	const Color& color,
	float lifetime
)
{
	VisDebugSphere shape {};
	shape.location = location;
	shape.radius = radius;
	shape.color = color;
	shape.set_lifetime( lifetime );

	push_shape( shape );
}

void VisDebug::add_line(
	const Vec3& start,
	const Vec3& end,
	const Color& color,
	float lifetime
)
{
	VisDebugLine shape {};
	shape.start = start;
	shape.end = end;
	shape.color = color;
	shape.set_lifetime( lifetime );

	push_shape( shape );
}

void VisDebug::render()
{
	auto& engine = Engine::instance();
	RenderBatch* render_batch = engine.get_render_batch();
	Updater* updater = engine.get_updater();

	for ( auto itr = shapes.begin(); itr != shapes.end(); )
	{
		const auto& shape = *itr;
		
		//	Check lifetime and erase when out-of-date
		if ( updater->get_accumulated_seconds() > shape->end_time )
		{
			itr = shapes.erase( itr );
			continue;
		}

		//	Render debug shape
		shape->render( render_batch );

		itr++;
	}
}
