#pragma once
#include <Core/System/SimulationCommon.hpp>
#include <Core/Unit/Components/IAbility.hpp>
#include <Core/Unit/Unit.hpp>
#include <Features/constants.hpp>
#include <IO/Events/UnitAttacked.hpp>
#include <random>

class Attack : public IAbility
{
public:
	Attack() = default;

	bool use(Unit& user) override
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
				static std::mt19937 rng{1993};
				std::uniform_int_distribution<uint32_t> dist(0, targetsInRange.size() - 1);
				if (auto target = targetsInRange[dist(rng)])
				{
					uint32_t oldHP = *(target->getStatValue(StatName::Health));
					if (uint32_t damage = getDamage(user, *target))
					{
						uint32_t newHP = damage > oldHP ? 0 : oldHP - damage;
						target->setStatValue(StatName::Health, newHP);
						logAttack(user, *target, damage, newHP);

						return true;
					}
				}
			}
		}

		return false;
	}

protected:
	inline void logAttack(Unit& user, Unit& target, uint32_t& damage, uint32_t& targetHp)
	{
		SimulationCommon::LogEvent(sw::io::UnitAttacked{user.getId(), target.getId(), damage, targetHp});
	}

protected:
	virtual bool canAttack(Unit&)
	{
		return true;
	}

	virtual bool targetFilter(Unit&)
	{
		return false;
	}

	virtual std::pair<uint32_t, uint32_t> getRange(Unit&)
	{
		return {1, 1};
	}

	virtual uint32_t getDamage(Unit&, Unit&)
	{
		return 1;
	}
};
