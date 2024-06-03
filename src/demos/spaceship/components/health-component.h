#pragma once

#include <suprengine/component.h>

namespace puzzle
{
	using namespace suprengine;

	class HealthComponent;

	struct DamageInfo
	{
		Entity* attacker { nullptr };

		float damage { 0.0f };
		Vec3 knockback { Vec3::zero };
	};

	struct DamageResult
	{
		DamageResult( const DamageInfo& info )
			: info( info ) {}

		const DamageInfo& info;
		std::weak_ptr<HealthComponent> victim;
		bool is_alive { false };
		bool is_valid { false };
	};

	class HealthComponent : public Component
	{
	public:
		HealthComponent( Entity* owner, float health = 100.0f );

		void heal( float amount );
		void heal_to_full();

		DamageResult damage( const DamageInfo& info );

		bool is_alive() const;

	public:
		float health { 0.0f };
		float max_health { 100.0f };

	public:
		/* 
		 * Called when the component received damage.
		 * 
		 * Parameters:
		 * - DamageResult& result
		 */
		Event<const DamageResult&> on_damage;

	private:
	};
}
