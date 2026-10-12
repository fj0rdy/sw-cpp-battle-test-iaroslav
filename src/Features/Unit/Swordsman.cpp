#include "Swordsman.hpp"

#include <Core/Components/ICommandSource.hpp>
#include <Core/Components/IUnitManager.hpp>
#include <Features/Ability/CrushingHit.hpp>
#include <Features/Ability/Movement.hpp>
#include <Features/constants.hpp>
#include <IO/Commands/SpawnSwordsman.hpp>

namespace sw::sim
{

	Swordsman::Swordsman(io::SpawnSwordsman& command) :
			BasicUnit(command.unitId)
	{
		setPosition(CellPoint(command.x, command.y));
		addStat(StatName::Health, command.hp);
		addStat(StatName::Strength, command.strength);
		addAbility<CrushingHit>();
		addAbility<Movement>();
	}

	const std::string& Swordsman::getType() const
	{
		static std::string type{"Swordsman"};
		return type;
	}

	void Swordsman::registerCommandHandlers(ICommandSource& commandSource, std::weak_ptr<IUnitManager> unitManager)
	{
		commandSource.add<io::SpawnSwordsman>(
			[unitManager](io::SpawnSwordsman command)
			{
				if (auto shared_units = unitManager.lock())
				{
					shared_units->addUnit(std::make_shared<Swordsman>(command));
				}
			});
	}

}  //namespace sw::sim
