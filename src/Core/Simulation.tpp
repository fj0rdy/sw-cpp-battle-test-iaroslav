#include <Core/System/SimulationCommon.hpp>
#include <Core/System/SimulationConfig.hpp>
#include <IO/System/TypeRegistry.hpp>

	template <class CommandCT, class MapCT, class UnitsCT>
    	requires SimulationComponents<CommandCT, MapCT, UnitsCT>
	Simulation<CommandCT, MapCT, UnitsCT>::Simulation(SimulationConfig& config) :
			_commandSource(std::make_shared<CommandCT>(config)),
			_mapManager(std::make_shared<MapCT>(config)),
			_unitManager(std::make_shared<UnitsCT>(config))
	{
		try
		{
			sw::TypeRegistry::Common().add<IMapManager>(_mapManager);
			sw::TypeRegistry::Common().add<IUnitManager>(_unitManager);

			_mapManager->registerCommandHandlers(_commandSource);
			_unitManager->registerCommandHandlers(_commandSource);
		}
		catch (...)
		{
			cleanup();
			throw;
		}
	}

	template <class CommandCT, class MapCT, class UnitsCT>
    	requires SimulationComponents<CommandCT, MapCT, UnitsCT>
	void Simulation<CommandCT, MapCT, UnitsCT>::cleanup()
	{
		sw::TypeRegistry::Common().remove<IMapManager>();
		sw::TypeRegistry::Common().remove<IUnitManager>();
		SimulationCommon::tickCounter.reset();
	}

	template <class CommandCT, class MapCT, class UnitsCT>
    	requires SimulationComponents<CommandCT, MapCT, UnitsCT>
	void Simulation<CommandCT, MapCT, UnitsCT>::progress()
	{
		SimulationCommon::tickCounter.set(++_tick);

		// if nothing to execute - progress simulation
		if (_commandSource->executeAll())
		{
			_hasProgress = true;
		}
		else
		{
			_mapManager->progress();
			_hasProgress = _unitManager->progress();
		}
	}