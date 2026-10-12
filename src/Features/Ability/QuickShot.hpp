#pragma once

#include <Core/System/SimulationCommon.hpp>
#include <Core/Unit/Unit.hpp>
#include <Features/Ability/Attack.hpp>
#include <Features/constants.hpp>

namespace sw::sim
{

	class QuickShot : public Attack
	{
	public:
		QuickShot() = default;

	protected:
		bool canAttack(Unit& user) override
		{
			if (auto units = SimulationCommon::UnitManager())
			{
				auto targetsInRange = units->getUnitsInRange(user.getPosition(), 1, 1);
				return targetsInRange.empty();
			}

			return false;
		}

		std::pair<Distance, Distance> getRange(Unit& user) override
		{
			auto range = user.getStatValue(StatName::Range);
			return {2, range ? *range : 0};
		}

		virtual Stat getDamage(Unit& user, Unit&) override
		{
			auto agl = user.getStatValue(StatName::Agility);
			return agl ? *agl : 0;
		}
	};

}  //namespace sw::sim
