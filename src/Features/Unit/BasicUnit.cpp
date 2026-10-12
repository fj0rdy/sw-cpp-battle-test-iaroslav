#include "BasicUnit.hpp"

#include <Features/Ability/Movement.hpp>
#include <Features/constants.hpp>

namespace sw::sim
{

	const std::string& BasicUnit::getType() const
	{
		static std::string type{"BasicUnit"};
		return type;
	}

	bool BasicUnit::isAlive() const
	{
		auto hp = getStatValue(StatName::Health);
		return hp ? *hp : true;
	}

	bool BasicUnit::occupiesSpace() const
	{
		return !hasTrait(TraitName::NonPhysical);
	}

	void BasicUnit::march(const CellPoint& point)
	{
		if (auto movement = getAbility<Movement>())
		{
			movement->setTarget(*this, point);
		}
	}

}  //namespace sw::sim
