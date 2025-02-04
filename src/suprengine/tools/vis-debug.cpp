#include "vis-debug.h"

#include <suprengine/core/engine.h>
#include <suprengine/core/assets.h>

using namespace suprengine;

DebugChannel VisDebug::active_channels = DebugChannel::None;

class VisDebugShape
{
public:
	Color color = Color::white;

	float end_time = 0.0f;

	DebugChannel channel = DebugChannel::None;

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


static std::vector<VisDebugBox> boxes {};
static std::vector<VisDebugSphere> spheres {};
static std::vector<VisDebugLine> lines {};


void VisDebug::add_box(
	const Vec3& location,
	const Quaternion& rotation,
	const Box& box,
	const Color& color,
	float lifetime,
	DebugChannel channel
)
{
	if ( !is_channel_active( channel ) ) return;

	VisDebugBox shape {};
	shape.location = location;
	shape.rotation = rotation;
	shape.box = box;
	shape.color = color;
	shape.channel = channel;
	shape.set_lifetime( lifetime );

	boxes.push_back( shape );
}

void VisDebug::add_sphere(
	const Vec3& location,
	float radius,
	const Color& color,
	float lifetime,
	DebugChannel channel
)
{
	if ( !is_channel_active( channel ) ) return;

	VisDebugSphere shape {};
	shape.location = location;
	shape.radius = radius;
	shape.color = color;
	shape.channel = channel;
	shape.set_lifetime( lifetime );

	spheres.push_back( shape );
}

void VisDebug::add_line(
	const Vec3& start,
	const Vec3& end,
	const Color& color,
	float lifetime,
	DebugChannel channel
)
{
	if ( !is_channel_active( channel ) ) return;

	VisDebugLine shape {};
	shape.start = start;
	shape.end = end;
	shape.color = color;
	shape.channel = channel;
	shape.set_lifetime( lifetime );

	lines.push_back( shape );
}

template <typename T>
void update_and_render_shapes( std::vector<T>& shapes, RenderBatch* render_batch, float current_time )
{
	for ( auto itr = shapes.begin(); itr != shapes.end(); )
	{
		T& shape = *itr;
		
		//	Check lifetime and erase when out-of-date
		if ( current_time > shape.end_time )
		{
			itr = shapes.erase( itr );
			continue;
		}

		if ( VisDebug::is_channel_active( shape.channel ) )
		{
			shape.render( render_batch );
		}

		itr++;
	}
}

void VisDebug::render()
{
	PROFILE_SCOPE( "VisDebug::render" );

	auto& engine = Engine::instance();
	RenderBatch* render_batch = engine.get_render_batch();

	Updater* updater = engine.get_updater();
	float current_time = updater->get_accumulated_seconds();

	update_and_render_shapes( boxes, render_batch, current_time );
	update_and_render_shapes( spheres, render_batch, current_time );
	update_and_render_shapes( lines, render_batch, current_time );
}

bool VisDebug::is_channel_active( DebugChannel channel )
{
	return ( active_channels & channel ) == channel;
}

size_t VisDebug::get_shapes_count()
{
	return boxes.size() + spheres.size() + lines.size();
}

size_t VisDebug::get_shapes_memory_usage()
{
	return sizeof( std::vector<VisDebugShape> ) * 3
		 + sizeof( VisDebugBox ) * boxes.capacity()
		 + sizeof( VisDebugSphere ) * spheres.capacity()
		 + sizeof( VisDebugLine ) * lines.capacity();
}
