#pragma once
#include <Core/Components/ICommandSource.hpp>
#include <Core/Components/IMapManager.hpp>
#include <Core/Components/IUnitManager.hpp>
#include <Core/System/SimulationCommon.hpp>
#include <Core/System/SimulationConfig.hpp>
#include <IO/System/TypeRegistry.hpp>
#include <cstdint>
#include <memory>

template <class C, class M, class U>
class Simulation
{
public:
	Simulation() = delete;
	Simulation(const Simulation&) = delete;
	Simulation& operator=(const Simulation&) = delete;
	Simulation(Simulation&&) = delete;
	Simulation& operator=(Simulation&&) = delete;

	Simulation(SimulationConfig& config) :
			_commandSource(std::make_shared<C>(config)),
			_mapManager(std::make_shared<M>(config)),
			_unitManager(std::make_shared<U>(config))
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

	~Simulation()
	{
		cleanup();
	}

	static void cleanup()
	{
		sw::TypeRegistry::Common().remove<IMapManager>();
		sw::TypeRegistry::Common().remove<IUnitManager>();
		SimulationCommon::tickCounter.reset();
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
	void progress()
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

private:
	std::shared_ptr<ICommandSource> _commandSource;
	std::shared_ptr<IMapManager> _mapManager;
	std::shared_ptr<IUnitManager> _unitManager;
	uint64_t _tick = 0;
	bool _hasProgress = false;
};
