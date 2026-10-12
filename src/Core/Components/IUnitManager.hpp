#pragma once

#include <Core/System/CellPoint.hpp>
#include <Core/System/types.hpp>
#include <IO/System/TypeRegistry.hpp>
#include <memory>
#include <vector>

namespace sw::sim
{

	class Unit;
	class SimulationConfig;
	class ICommandSource;

	namespace sw::io
	{
		class March;
	}

	class IUnitManager
	{
	public:
		IUnitManager(SimulationConfig&) {}

		virtual ~IUnitManager() = default;

		virtual void registerCommandHandlers(std::shared_ptr<ICommandSource>) = 0;
		virtual bool progress() = 0;

		virtual void addUnit(std::shared_ptr<Unit>) = 0;
		virtual std::shared_ptr<Unit> getUnit(Id id) = 0;
		virtual std::vector<std::shared_ptr<Unit>> getUnitsInRange(
			const CellPoint&, Distance minRange, Distance maxRange) = 0;

		virtual bool isCellAvailible(const CellPoint&) = 0;
	};

}  //namespace sw::sim
