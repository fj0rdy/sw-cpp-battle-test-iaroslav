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

void BasicMapManager::registerCommandHandlers(std::shared_ptr<ICommandSource> commandSource)
{
	if (commandSource)
	{
		auto weak_this = weak_from_this();
		commandSource->add<sw::io::CreateMap>(
			[weak_this](auto command)
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

uint32_t BasicMapManager::distance(const CellPoint& p1, const CellPoint& p2)
{
	uint32_t xdif = std::max(p1.x, p2.x) - std::min(p1.x, p2.x);
	uint32_t ydif = std::max(p1.y, p2.y) - std::min(p1.y, p2.y);
	return std::max(xdif, ydif);
}

void BasicMapManager::clampPoint(CellPoint& p)
{
	if (!isInitialized())
	{
		// TODO: warning log
		return;
	}

	p.x = std::min(p.x, _width - 1);
	p.y = std::min(p.y, _height - 1);
}

void BasicMapManager::advancePointToTarget(CellPoint& p, const CellPoint& target)
{
	if (!isInitialized())
	{
		// TODO: warning log
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

void BasicMapManager::createMap(const sw::io::CreateMap& command)
{
	if (!isInitialized())
	{
		resize(command.width, command.height);
		logMapCreated();
	}
	else
	{
		// TODO: warning log
	}
}

void BasicMapManager::logMapCreated()
{
	SimulationCommon::LogEvent(sw::io::MapCreated{_width, _height});
}