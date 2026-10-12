#include "BasicMapManager.hpp"

#include <Core/Components/ICommandSource.hpp>
#include <Core/Components/IMapManager.hpp>
#include <Core/System/SimulationCommon.hpp>
#include <IO/Commands/CreateMap.hpp>
#include <IO/Events/MapCreated.hpp>
#include <IO/System/EventLog.hpp>
#include <algorithm>
#include <cmath>
#include <memory>

namespace sw::sim
{

	void BasicMapManager::registerCommandHandlers(std::shared_ptr<ICommandSource> commandSource)
	{
		if (commandSource)
		{
			auto weak_this = weak_from_this();
			commandSource->add<io::CreateMap>(
				[weak_this](io::CreateMap command)
				{
					if (auto shared_this = weak_this.lock())
					{
						shared_this->createMap(command);
					}
				});
		}
	}

	bool BasicMapManager::isCellValid(const CellPoint& p)
	{
		return p.x < _width && p.y < _height;
	}

	bool BasicMapManager::isCellAvailible(const CellPoint& p)
	{
		return isCellValid(p);
	}

	Distance BasicMapManager::distance(const CellPoint& p1, const CellPoint& p2)
	{
		Distance xdif = std::max(p1.x, p2.x) - std::min(p1.x, p2.x);
		Distance ydif = std::max(p1.y, p2.y) - std::min(p1.y, p2.y);
		return std::max(xdif, ydif);
	}

	void BasicMapManager::clampPoint(CellPoint& p)
	{
		if (!isInitialized())
		{
			SimulationCommon::logWarning("calling clampPoint() before map initialization!");
			return;
		}

		p.x = std::min(p.x, _width - 1);
		p.y = std::min(p.y, _height - 1);
	}

	void BasicMapManager::advancePointToTarget(CellPoint& p, const CellPoint& target)
	{
		if (!isInitialized())
		{
			SimulationCommon::logWarning("calling advancePointToTarget() before map initialization!");
			return;
		}

		if (p.x != target.x)
		{
			p.x += target.x > p.x ? 1 : -1;
		}

		if (p.y != target.y)
		{
			p.y += target.y > p.y ? 1 : -1;
		}
	}

	void BasicMapManager::createMap(const io::CreateMap& command)
	{
		if (!isInitialized())
		{
			resize(command.width, command.height);
			logMapCreated();
		}
		else
		{
			SimulationCommon::logWarning("repeated map initialization!");
		}
	}

	void BasicMapManager::logMapCreated()
	{
		SimulationCommon::logEvent(io::MapCreated{_width, _height});
	}

}  //namespace sw::sim
