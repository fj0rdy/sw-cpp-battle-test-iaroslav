#pragma once
#include <Core/Components/ICommandSource.hpp>
#include <Core/Components/IMapManager.hpp>
#include <Core/System/SimulationCommon.hpp>
#include <IO/Commands/CreateMap.hpp>
#include <IO/Events/MapCreated.hpp>
#include <IO/System/EventLog.hpp>
#include <algorithm>
#include <cmath>
#include <memory>

namespace sw::io
{
	struct CreateMap;
}

class BasicMapManager : public IMapManager, public std::enable_shared_from_this<BasicMapManager>
{
public:
	BasicMapManager(SimulationConfig& config) :
			IMapManager(config)
	{}

	void registerCommandHandlers(std::shared_ptr<ICommandSource> commandSource) override
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

	void progress() override {}

	bool isCellValid(const CellPoint& p) override
	{
		return p.x < _width && p.y < _height;
	}

	bool isCellAvailible(const CellPoint& p) override
	{
		return isCellValid(p);
	}

	uint32_t distance(const CellPoint& p1, const CellPoint& p2) override
	{
		uint32_t xdif = std::max(p1.x, p2.x) - std::min(p1.x, p2.x);
		uint32_t ydif = std::max(p1.y, p2.y) - std::min(p1.y, p2.y);
		return std::max(xdif, ydif);
	}

	void clampPoint(CellPoint& p) override
	{
		if (!isInitialized())
		{
			// TODO: warning log
			return;
		}

		p.x = std::min(p.x, _width - 1);
		p.y = std::min(p.y, _height - 1);
	}

	void advancePointToTarget(CellPoint& p, const CellPoint& target) override
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

protected:
	void createMap(const sw::io::CreateMap& command)
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

	void logMapCreated()
	{
		SimulationCommon::LogEvent(sw::io::MapCreated{_width, _height});
	}
};
