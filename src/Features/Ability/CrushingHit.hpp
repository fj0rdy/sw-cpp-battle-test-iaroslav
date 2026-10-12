#pragma once
#include <Core/Unit/Unit.hpp>
#include <Features/Ability/Attack.hpp>
#include <Features/constants.hpp>


class CrushingHit : public Attack
{
public:
	CrushingHit() :
			Attack()
	{}

protected:
	uint32_t getDamage(Unit& user, Unit&) override
	{
		auto strg = user.getStatValue(StatName::Strength);
		return strg ? *strg : 0;
	}
};
