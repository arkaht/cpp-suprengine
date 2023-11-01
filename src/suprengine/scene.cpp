#include "scene.h"

#include "game.h"

using namespace suprengine;

Scene::Scene()
	: _game( &Game::instance() ) 
{}
