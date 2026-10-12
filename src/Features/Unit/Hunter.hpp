#pragma once

#include <Features/Unit/BasicUnit.hpp>
#include <memory>

namespace sw::io
{
	struct SpawnHunter;
}

namespace sw::sim
{

	class ICommandSource;
	class IUnitManager;

	class Hunter : public BasicUnit
	{
	public:
		Hunter(io::SpawnHunter& command);

		const std::string& getType() const override;

		static void registerCommandHandlers(ICommandSource& commandSource, std::weak_ptr<IUnitManager> unitManager);
	};

}  //namespace sw::sim
