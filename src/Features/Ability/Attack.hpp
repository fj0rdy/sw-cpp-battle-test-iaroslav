#pragma once

#include <Core/System/types.hpp>
#include <Core/Unit/Components/IAbility.hpp>
#include <cstdint>
#include <utility>

namespace sw::sim
{

	class Unit;

	class Attack : public IAbility
	{
	public:
		Attack() = default;

		bool use(Unit& user) override;

	protected:
		virtual bool canAttack(Unit&);
		virtual bool targetFilter(Unit&);
		virtual std::pair<Distance, Distance> getRange(Unit&);
		virtual Stat getDamage(Unit&, Unit&);

		void logAttack(Unit& user, Unit& target, Stat& damage, Stat& targetHp);
	};

}  //namespace sw::sim
