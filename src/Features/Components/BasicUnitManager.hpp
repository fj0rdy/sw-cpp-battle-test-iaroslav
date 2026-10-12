#pragma once
#include <Core/Components/ICommandSource.hpp>
#include <Core/Components/IUnitManager.hpp>
#include <Core/System/SimulationCommon.hpp>
#include <Core/Unit/Unit.hpp>
#include <IO/Commands/March.hpp>
#include <IO/Events/UnitDied.hpp>
#include <IO/Events/UnitSpawned.hpp>
#include <algorithm>
#include <memory>

template <class... UnitTypes>
class BasicUnitmanager : public IUnitManager, public std::enable_shared_from_this<BasicUnitmanager<UnitTypes...>>
{
public:
	BasicUnitmanager(SimulationConfig& config) :
			IUnitManager(config)
	{}

	void registerCommandHandlers(std::shared_ptr<ICommandSource> commandSource) override
	{
		auto weak_this = this->weak_from_this();
		(UnitTypes::registerCommandHandlers(*commandSource, weak_this), ...);

		commandSource->add<sw::io::March>(
			[weak_this](auto command)
			{
				if (auto shared_this = weak_this.lock())
				{
					shared_this->marchUnit(command);
				}
			});
	}

	bool progress() override
	{
		if (_units.size() < 2)
		{
			return false;
		}

		bool success = false;
		for (auto& unit : _units)
		{
			success |= unit->progress();
		}

		removeDeadUnits();

		return success;
	}

	void addUnit(std::shared_ptr<Unit> unit) override
	{
		if (unit && !getUnit(unit->getId()))
		{
			_units.push_back(unit);
			logUnitSpawned(*unit);
		}
	}

	std::shared_ptr<Unit> getUnit(uint32_t id) override
	{
		auto it = std::find_if(_units.begin(), _units.end(), [id](auto it) { return it->getId() == id; });
		return it != _units.end() ? *it : std::shared_ptr<Unit>();
	}

	bool isCellAvailible(const CellPoint& p) override
	{
		return std::find_if(
				   _units.begin(),
				   _units.end(),
				   [&p](auto it) { return it->occupiesSpace() && it->getPosition() == p; })
			   == _units.end();
	}

	std::vector<std::shared_ptr<Unit>> getUnitsInRange(
		const CellPoint& point, uint32_t minRange, uint32_t maxRange) override
	{
		std::vector<std::shared_ptr<Unit>> result;
		if (auto map = SimulationCommon::MapManager())
		{
			for (const auto& unit : _units)
			{
				auto dist = map->distance(point, unit->getPosition());
				if (dist >= minRange && dist <= maxRange)
				{
					result.push_back(unit);
				}
			}
		}

		return result;
	}

protected:
	virtual void removeDeadUnits()
	{
		for (auto it = _units.begin(); it != _units.end();)
		{
			if (!*it || !(*it)->isAlive())
			{
				beforeRemoveDeadUnit(*it);
				it = _units.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	virtual void beforeRemoveDeadUnit(Unit::Ptr& deadUnit)
	{
		if (deadUnit)
		{
			logUnitDied(deadUnit->getId());
		}
	}

	virtual void marchUnit(sw::io::March command)
	{
		if (auto map = SimulationCommon::MapManager())
		{
			if (auto unit = getUnit(command.unitId))
			{
				CellPoint point(command.targetX, command.targetY);
				map->clampPoint(point);
				unit->march(point);
			}
		}
	}

	void logUnitDied(uint32_t id)
	{
		SimulationCommon::LogEvent(sw::io::UnitDied{id});
	}

	void logUnitSpawned(Unit& unit)
	{
		SimulationCommon::LogEvent(
			sw::io::UnitSpawned{unit.getId(), unit.getType(), unit.getPosition().x, unit.getPosition().y});
	}

private:
	std::vector<std::shared_ptr<Unit>> _units;
};
