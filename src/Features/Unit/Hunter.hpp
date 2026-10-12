#pragma once
#include <Core/Components/ICommandSource.hpp>
#include <Core/Components/IUnitManager.hpp>
#include <Features/Ability/Movement.hpp>
#include <Features/Ability/QuickShot.hpp>
#include <Features/Ability/ShadowStrike.hpp>
#include <Features/Unit/BasicUnit.hpp>
#include <Features/constants.hpp>
#include <IO/Commands/SpawnHunter.hpp>
#include <memory>

class Hunter : public BasicUnit
{
public:
	Hunter(sw::io::SpawnHunter& command) :
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

	const std::string& getType() const override
	{
		static std::string type{"Hunter"};
		return type;
	}

	static void registerCommandHandlers(ICommandSource& commandSource, std::weak_ptr<IUnitManager> unitManager)
	{
		commandSource.add<sw::io::SpawnHunter>(
			[unitManager](sw::io::SpawnHunter command)
			{
				if (auto shared_units = unitManager.lock())
				{
					shared_units->addUnit(std::make_shared<Hunter>(command));
				}
			});
	}
};
