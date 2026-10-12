#pragma once

#include <Core/System/details/concepts.hpp>
#include <Core/System/types.hpp>
#include <cstdint>

namespace sw::sim
{

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
		friend class Simulation;

	public:
		Tick get()
		{
			return _tickCount;
		}

	protected:
		void set(Tick value)
		{
			_tickCount = value;
		}

		void reset()
		{
			_tickCount = 0;
		}

	private:
		Tick _tickCount = 0;
	};

}  //namespace sw::sim
