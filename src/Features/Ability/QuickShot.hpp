#pragma once
#include <Core/System/SimulationCommon.hpp>
#include <Features/Ability/Attack.hpp>
#include <Features/constants.hpp>

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

	std::pair<uint32_t, uint32_t> getRange(Unit& user) override
	{
		auto range = user.getStatValue(StatName::Range);
		return {2, range ? *range : 0};
	}

	virtual uint32_t getDamage(Unit& user, Unit&) override
	{
		auto agl = user.getStatValue(StatName::Agility);
		return agl ? *agl : 0;
	}
};
