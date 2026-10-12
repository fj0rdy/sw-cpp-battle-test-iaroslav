#pragma once

#include <Core/Unit/Unit.hpp>
#include <Features/Ability/Attack.hpp>
#include <Features/constants.hpp>

namespace sw::sim
{

	class CrushingHit : public Attack
	{
	public:
		CrushingHit() :
				Attack()
		{}

	protected:
		Stat getDamage(Unit& user, Unit&) override
		{
			auto strg = user.getStatValue(StatName::Strength);
			return strg ? *strg : 0;
		}
	};

}  //namespace sw::sim
