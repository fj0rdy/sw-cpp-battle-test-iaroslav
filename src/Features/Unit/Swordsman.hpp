#pragma once

#include <Features/Unit/BasicUnit.hpp>
#include <memory>

namespace sw::io
{
	struct SpawnSwordsman;
}

namespace sw::sim
{

	class ICommandSource;
	class IUnitManager;

	class Swordsman : public BasicUnit
	{
	public:
		Swordsman(io::SpawnSwordsman& command);

		const std::string& getType() const override;

		static void registerCommandHandlers(ICommandSource& commandSource, std::weak_ptr<IUnitManager> unitManager);
	};

}  //namespace sw::sim
