#pragma once

#include <Core/Components/IUnitManager.hpp>
#include <Core/Unit/Unit.hpp>
#include <concepts>
#include <memory>

namespace sw::io
{
	struct March;
}

namespace sw::sim
{

	class ICommandSource;

	template <class UnitT>
	concept RegistrableUnit
		= std::derived_from<UnitT, Unit> && requires(ICommandSource& source, std::weak_ptr<IUnitManager> weak) {
			  { UnitT::registerCommandHandlers(source, weak) } -> std::same_as<void>;
		  };

	template <RegistrableUnit... UnitTypes>
	class BasicUnitmanager : public IUnitManager, public std::enable_shared_from_this<BasicUnitmanager<UnitTypes...>>
	{
	public:
		BasicUnitmanager(SimulationConfig& config) :
				IUnitManager(config)
		{}

		void registerCommandHandlers(std::shared_ptr<ICommandSource> commandSource) override;

		bool progress() override;

		void addUnit(std::shared_ptr<Unit> unit) override;

		std::shared_ptr<Unit> getUnit(Id id) override;

		bool isCellAvailible(const CellPoint& p) override;

		std::vector<std::shared_ptr<Unit>> getUnitsInRange(
			const CellPoint& point, Distance minRange, Distance maxRange) override;

	protected:
		void removeDeadUnits();

		virtual void registerMarchCommand(ICommandSource& commandSource);

		virtual void beforeRemoveDeadUnit(Unit::Ptr& deadUnit);

		virtual void marchUnit(io::March& command);

		void logUnitDied(Id id);

		void logUnitSpawned(Unit& unit);

	private:
		std::vector<std::shared_ptr<Unit>> _units;
	};

}  //namespace sw::sim

#include "BasicUnitManager.tpp"
