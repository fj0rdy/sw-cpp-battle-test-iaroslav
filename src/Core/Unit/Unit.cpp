#include "Unit.hpp"
#include <Core/System/SimulationCommon.hpp>

const std::string& Unit::getType() const
{
	static std::string type{};
	return type;
}

const CellPoint& Unit::getPosition() const
{
	return _position;
}

void Unit::setPosition(const CellPoint& position)
{
	if (auto map = SimulationCommon::MapManager())
	{
		CellPoint newPos(position);
		map->clampPoint(newPos);
		_position = newPos;
	}
}

bool Unit::isAlive() const
{
	return true;
}

bool Unit::occupiesSpace() const
{
	return true;
}

bool Unit::progress()
{
	for (auto& ability : _abilities)
	{
		if (ability && ability->use(*this))
		{
			return true;
		}
	}

	return false;
}

bool Unit::hasStat(const std::string& name) const
{
	return _stats.find(name) != _stats.end();
}

Unit::OptStat Unit::getStatValue(const std::string& name) const
{
	auto it = _stats.find(name);
	if (it != _stats.end())
	{
		return it->second.second;
	}

	return OptStat{};
}

void Unit::setStatValue(const std::string& name, statValue value)
{
	auto it = _stats.find(name);
	if (it != _stats.end())
	{
		it->second.second = value;
	}
}

Unit::OptStat Unit::getStatBase(const std::string& name) const
{
	auto it = _stats.find(name);
	if (it != _stats.end())
	{
		return it->second.first;
	}

	return OptStat{};
}

bool Unit::hasTrait(const std::string& name) const
{
	return _traits.contains(name);
}

Unit& Unit::addStat(const std::string& name, uint32_t value)
{
	_stats[name] = {value, value};
	return *this;
}

Unit& Unit::addTrait(const std::string& trait)
{
	_traits.emplace(trait);
	return *this;
}
