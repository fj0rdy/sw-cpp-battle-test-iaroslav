#pragma once
#include <Core/Components/ICommandSource.hpp>
#include <Core/Components/IUnitManager.hpp>
#include <Core/System/SimulationCommon.hpp>
#include <Core/Unit/Unit.hpp>
#include <IO/Commands/March.hpp>
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
		(UnitTypes::registerCommandHandlers(commandSource, weak_this), ...);

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

		std::erase_if(_units, [](const std::shared_ptr<Unit>& unit) -> bool { return !unit || !unit->isAlive(); });

		return success;
	}

	void addUnit(std::shared_ptr<Unit> unit) override
	{
		if (unit && !getUnit(unit->getId()))
		{
			_units.push_back(unit);
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

	std::vector<std::shared_ptr<Unit>> getUnitsInRange(const CellPoint& p, uint32_t range) override
	{
		std::vector<std::shared_ptr<Unit>> result;
		for (const auto& unit : _units)
		{
			const CellPoint& position = unit->getPosition();
			auto xdif = std::max(p.x, position.x) - std::min(p.x, position.x);
			auto ydif = std::max(p.y, position.y) - std::min(p.y, position.y);
			if (std::max(xdif, ydif) <= range)
			{
				result.push_back(unit);
			}
		}

		return result;
	}

protected:
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

private:
	std::vector<std::shared_ptr<Unit>> _units;
};
