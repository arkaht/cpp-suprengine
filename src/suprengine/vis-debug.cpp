#include "vis-debug.h"

#include <suprengine/engine.h>

using namespace suprengine;

void VisDebug::add_line(
	const Vec3& start,
	const Vec3& end,
	const Color& color,
	float lifetime
)
{
	Updater* updater = Engine::instance().get_updater();
	_lines.emplace_back(
		VisDebugLine { 
			start, end, 
			color, 
			updater->get_accumulated_seconds() + lifetime
		}
	);
}

void VisDebug::render()
{
	auto& engine = Engine::instance();
	RenderBatch* render_batch = engine.get_render_batch();
	Updater* updater = engine.get_updater();

	for ( auto itr = _lines.begin(); itr != _lines.end(); )
	{
		const VisDebugLine& line = *itr;
		
		//	Check lifetime and erase when out-of-date
		if ( updater->get_accumulated_seconds() > line.end_time )
		{
			itr = _lines.erase( itr );
			continue;
		}

		//  Draw the actual line
		render_batch->draw_line( line.start, line.end, line.color );

		itr++;
	}
}
