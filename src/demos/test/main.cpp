#include <suprengine/core/engine.h>

#include "game-instance.h"

using namespace suprengine;

int main( int arg_count, char** args )
{
	auto& engine = Engine::instance();
	return engine.run<test::GameInstance>();
}