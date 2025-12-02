#include "scene.h"

#include "game.h"
#include "engine.h"

using namespace suprengine;

Scene::Scene() : _game( Engine::instance().get_game() )
{
}

void Scene::render( RenderBatch* render_batch, const SharedPtr<Camera>& camera )
{
	render_batch->render( camera );
}
