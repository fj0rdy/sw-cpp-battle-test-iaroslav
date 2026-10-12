#pragma once
#include <Features/Unit/BasicUnit.hpp>
#include <memory>

class ICommandSource;
class IUnitManager;

namespace sw::io
{
    class SpawnSwordsman;
}

class Swordsman : public BasicUnit
{
public:
	Swordsman(sw::io::SpawnSwordsman& command);

	const std::string& getType() const override;

	static void registerCommandHandlers(ICommandSource& commandSource, std::weak_ptr<IUnitManager> unitManager);
};
