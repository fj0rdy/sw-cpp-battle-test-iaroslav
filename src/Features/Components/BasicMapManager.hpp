#pragma once

#include <Core/Components/IMapManager.hpp>
#include <memory>

namespace sw::io
{
	struct CreateMap;
}

namespace sw::sim
{

	class ICommandSource;

	class BasicMapManager : public IMapManager, public std::enable_shared_from_this<BasicMapManager>
	{
	public:
		BasicMapManager(SimulationConfig& config) :
				IMapManager(config)
		{}

		void registerCommandHandlers(std::shared_ptr<ICommandSource> commandSource) override;

		void progress() override {}

		bool isCellValid(const CellPoint& p) override;
		bool isCellAvailible(const CellPoint& p) override;

		Distance distance(const CellPoint& p1, const CellPoint& p2) override;
		void clampPoint(CellPoint& p) override;
		void advancePointToTarget(CellPoint& p, const CellPoint& target) override;

	protected:
		void createMap(const io::CreateMap& command);
		void logMapCreated();
	};

}  //namespace sw::sim
