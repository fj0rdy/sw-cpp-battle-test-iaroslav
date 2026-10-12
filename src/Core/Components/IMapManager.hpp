#pragma once
#include <Core/System/CellPoint.hpp>
#include <cstdint>
#include <memory>

class SimulationConfig;
class ICommandSource;

class IMapManager
{
public:
	IMapManager(SimulationConfig&) {}

	virtual ~IMapManager() = default;

	bool isInitialized()
	{
		return _width && _height;
	}

	virtual void registerCommandHandlers(std::shared_ptr<ICommandSource>) = 0;
	virtual void progress() = 0;

	virtual bool isCellValid(const CellPoint&) = 0;
	virtual bool isCellAvailible(const CellPoint&) = 0;
	virtual uint32_t distance(const CellPoint&, const CellPoint&) = 0;
	virtual void clampPoint(CellPoint&) = 0;
	virtual void advancePointToTarget(CellPoint&, const CellPoint&) = 0;

protected:
	void resize(uint32_t width, uint32_t height)
	{
		_width = width;
		_height = height;
	}

protected:
	uint32_t _width = 0;
	uint32_t _height = 0;
};
