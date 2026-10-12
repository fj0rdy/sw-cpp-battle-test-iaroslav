#pragma once
#include <Core/Components/ICommandSource.hpp>
#include <Core/Components/IMapManager.hpp>
#include <Core/Components/IUnitManager.hpp>
#include <Core/System/details/concepts.hpp>
#include <concepts>
#include <cstdint>
#include <memory>

namespace sw::sim
{

	class SimulationConfig;

	template <class CommandCT, class MapCT, class UnitsCT>
		requires SimulationComponents<CommandCT, MapCT, UnitsCT>
	class Simulation
	{
	public:
		Simulation() = delete;
		Simulation(const Simulation&) = delete;
		Simulation& operator=(const Simulation&) = delete;
		Simulation(Simulation&&) = delete;
		Simulation& operator=(Simulation&&) = delete;

		Simulation(SimulationConfig& config);

		~Simulation()
		{
			cleanup();
		}

		void start()
		{
			do
			{
				progress();
			}
			while (_hasProgress);
		}

	private:
		void progress();

		static void cleanup();

	private:
		std::shared_ptr<ICommandSource> _commandSource;
		std::shared_ptr<IMapManager> _mapManager;
		std::shared_ptr<IUnitManager> _unitManager;
		Tick _tick = 0;
		bool _hasProgress = false;
	};

}  //namespace sw::sim

#include "Simulation.tpp"
