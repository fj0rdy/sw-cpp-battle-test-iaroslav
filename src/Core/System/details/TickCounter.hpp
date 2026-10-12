#pragma once

#include <Core/System/details/concepts.hpp>
#include <cstdint>

class ICommandSource;
class IMapManager;
class IUnitManager;

template <class CommandCT, class MapCT, class UnitsCT>
    requires SimulationComponents<CommandCT, MapCT, UnitsCT>
class Simulation;

class TickCounter
{
	template <class CommandCT, class MapCT, class UnitsCT>
        requires SimulationComponents<CommandCT, MapCT, UnitsCT>
	friend class ::Simulation;

public:
	uint64_t get()
	{
		return _tickCount;
	}

protected:
	void set(uint64_t value)
	{
		_tickCount = value;
	}

	void reset()
	{
		_tickCount = 0;
	}

private:
	uint64_t _tickCount = 0;
};
