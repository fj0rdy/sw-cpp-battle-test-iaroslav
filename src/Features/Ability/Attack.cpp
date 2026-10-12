#include "Attack.hpp"

#include <Core/System/SimulationCommon.hpp>
#include <Core/Unit/Unit.hpp>
#include <Features/constants.hpp>
#include <IO/Events/UnitAttacked.hpp>
#include <random>

namespace sw::sim
{

	bool Attack::use(Unit& user)
	{
		if (!canAttack(user))
		{
			return false;
		}

		if (auto units = SimulationCommon::UnitManager())
		{
			auto [minRange, maxRange] = getRange(user);
			auto targetsInRange = units->getUnitsInRange(user.getPosition(), minRange, maxRange);
			std::erase_if(
				targetsInRange,
				[this, &user](const auto& unit)
				{
					return !unit || unit->getId() == user.getId() || !unit->hasStat(StatName::Health)
						   || !*(unit->getStatValue(StatName::Health)) || this->targetFilter(*unit);
				});

			if (!targetsInRange.empty())
			{
				static std::mt19937 rng{SimulationCommon::randomSeed};
				std::uniform_int_distribution<size_t> dist(0, targetsInRange.size() - 1);
				if (auto target = targetsInRange[dist(rng)])
				{
					Stat oldHP = *(target->getStatValue(StatName::Health));
					if (Stat damage = getDamage(user, *target))
					{
						Stat newHP = damage > oldHP ? 0 : oldHP - damage;
						target->setStatValue(StatName::Health, newHP);
						logAttack(user, *target, damage, newHP);

						return true;
					}
				}
			}
		}

		return false;
	}

	void Attack::logAttack(Unit& user, Unit& target, Stat& damage, Stat& targetHp)
	{
		SimulationCommon::logEvent(io::UnitAttacked{user.getId(), target.getId(), damage, targetHp});
	}

	bool Attack::canAttack(Unit&)
	{
		return true;
	}

	bool Attack::targetFilter(Unit&)
	{
		return false;
	}

	std::pair<Distance, Distance> Attack::getRange(Unit&)
	{
		return {1, 1};
	}

	Stat Attack::getDamage(Unit&, Unit&)
	{
		return 1;
	}

}  //namespace sw::sim
