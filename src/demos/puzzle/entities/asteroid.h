#pragma once
#include <components/stylized-model-renderer.hpp>

namespace puzzle
{
	using namespace suprengine;

	class Asteroid : public Entity
	{
	public:
		Asteroid();
		~Asteroid();

		void update_this( float dt ) override;

		Vec3 linear_direction = Vec3::forward * 5.0f;

	private:
		std::shared_ptr<StylizedModelRenderer> _model_renderer;
	};
}