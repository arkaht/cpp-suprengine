#include "scene.h"

#include "game.h"
#include "engine.h"

using namespace suprengine;

Scene::Scene()
	: _game( Engine::instance().get_game() ) 
{}
