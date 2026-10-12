#include "Hunter.hpp"

#include <Core/Components/ICommandSource.hpp>
#include <Core/Components/IUnitManager.hpp>
#include <Features/Ability/Movement.hpp>
#include <Features/Ability/QuickShot.hpp>
#include <Features/Ability/ShadowStrike.hpp>
#include <Features/constants.hpp>
#include <IO/Commands/SpawnHunter.hpp>

namespace sw::sim
{

	Hunter::Hunter(io::SpawnHunter& command) :
			BasicUnit(command.unitId)
	{
		setPosition(CellPoint(command.x, command.y));
		addStat(StatName::Health, command.hp);
		addStat(StatName::Strength, command.strength);
		addStat(StatName::Agility, command.agility);
		addStat(StatName::Range, command.range);
		addAbility<QuickShot>();
		addAbility<ShadowStrike>();
		addAbility<Movement>();
	}

	const std::string& Hunter::getType() const
	{
		static std::string type{"Hunter"};
		return type;
	}

	void Hunter::registerCommandHandlers(ICommandSource& commandSource, std::weak_ptr<IUnitManager> unitManager)
	{
		commandSource.add<io::SpawnHunter>(
			[unitManager](io::SpawnHunter command)
			{
				if (auto shared_units = unitManager.lock())
				{
					shared_units->addUnit(std::make_shared<Hunter>(command));
				}
			});
	}

}  //namespace sw::sim
