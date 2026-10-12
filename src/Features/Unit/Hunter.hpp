#pragma once
#include <Features/Unit/BasicUnit.hpp>
#include <memory>

class ICommandSource;
class IUnitManager;

namespace sw::io
{
    class SpawnHunter;
}

class Hunter : public BasicUnit
{
public:
	Hunter(sw::io::SpawnHunter& command);

	const std::string& getType() const override;

	static void registerCommandHandlers(ICommandSource& commandSource, std::weak_ptr<IUnitManager> unitManager);
};
