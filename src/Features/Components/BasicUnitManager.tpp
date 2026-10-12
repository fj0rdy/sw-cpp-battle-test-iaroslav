#include <Core/Components/ICommandSource.hpp>
#include <Core/System/SimulationCommon.hpp>
#include <IO/Commands/March.hpp>
#include <IO/Events/UnitDied.hpp>
#include <IO/Events/UnitSpawned.hpp>
#include <algorithm>

template <RegistrableUnit... UnitTypes>
void BasicUnitmanager<UnitTypes...>::registerCommandHandlers(std::shared_ptr<ICommandSource> commandSource)
{
	auto weak_this = this->weak_from_this();
	(UnitTypes::registerCommandHandlers(*commandSource, weak_this), ...);

	registerMarchCommand(*commandSource);
}

template <RegistrableUnit... UnitTypes>
bool BasicUnitmanager<UnitTypes...>::progress()
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

template <RegistrableUnit... UnitTypes>
void BasicUnitmanager<UnitTypes...>::addUnit(std::shared_ptr<Unit> unit)
{
	if (unit && !getUnit(unit->getId()))
	{
		_units.push_back(unit);
		logUnitSpawned(*unit);
	}
}

template <RegistrableUnit... UnitTypes>
std::shared_ptr<Unit> BasicUnitmanager<UnitTypes...>::getUnit(uint32_t id)
{
	auto it = std::find_if(_units.begin(), _units.end(), [id](auto it) { return it->getId() == id; });
	return it != _units.end() ? *it : std::shared_ptr<Unit>();
}

template <RegistrableUnit... UnitTypes>
bool BasicUnitmanager<UnitTypes...>::isCellAvailible(const CellPoint& p)
{
	return std::find_if(
				_units.begin(),
				_units.end(),
				[&p](auto it) { return it->occupiesSpace() && it->getPosition() == p; })
			== _units.end();
}

template <RegistrableUnit... UnitTypes>
std::vector<std::shared_ptr<Unit>> BasicUnitmanager<UnitTypes...>::getUnitsInRange(
	const CellPoint& point, uint32_t minRange, uint32_t maxRange)
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

template <RegistrableUnit... UnitTypes>
void BasicUnitmanager<UnitTypes...>::registerMarchCommand(ICommandSource& commandSource)
{
	auto weak_this = this->weak_from_this();
	commandSource.add<sw::io::March>(
		[weak_this](auto command)
		{
			if (auto shared_this = weak_this.lock())
			{
				shared_this->marchUnit(command);
			}
		});
} 

template <RegistrableUnit... UnitTypes>
void BasicUnitmanager<UnitTypes...>::removeDeadUnits()
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

template <RegistrableUnit... UnitTypes>
void BasicUnitmanager<UnitTypes...>::beforeRemoveDeadUnit(Unit::Ptr& deadUnit)
{
	if (deadUnit)
	{
		logUnitDied(deadUnit->getId());
	}
}

template <RegistrableUnit... UnitTypes>
void BasicUnitmanager<UnitTypes...>::marchUnit(sw::io::March& command)
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

template <RegistrableUnit... UnitTypes>
void BasicUnitmanager<UnitTypes...>::logUnitDied(uint32_t id)
{
	SimulationCommon::LogEvent(sw::io::UnitDied{id});
}

template <RegistrableUnit... UnitTypes>
void BasicUnitmanager<UnitTypes...>::logUnitSpawned(Unit& unit)
{
	SimulationCommon::LogEvent(
		sw::io::UnitSpawned{unit.getId(), unit.getType(), unit.getPosition().x, unit.getPosition().y});
}