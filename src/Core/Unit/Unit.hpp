#pragma once

#include <Core/System/CellPoint.hpp>
#include <Core/System/SimulationCommon.hpp>
#include <Core/Unit/Components/IAbility.hpp>
#include <algorithm>
#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

class Unit
{
public:
	using Ptr = std::shared_ptr<Unit>;
	using statValue = uint32_t;
	using statBaseValue = statValue;
	using OptStat = std::optional<statValue>;

public:
	explicit Unit(uint32_t id) :
			_id(id)
	{}

	virtual ~Unit() = default;

	uint32_t getId()
	{
		return _id;
	}

	virtual const std::string& getType() const
	{
		static std::string type{};
		return type;
	}

	virtual const CellPoint& getPosition() const
	{
		return _position;
	}

	virtual void setPosition(const CellPoint& position)
	{
		if (auto map = SimulationCommon::MapManager())
		{
			CellPoint newPos(position);
			map->clampPoint(newPos);
			_position = newPos;
		}
	}

	virtual bool isAlive() const
	{
		return true;
	}

	virtual bool occupiesSpace() const
	{
		return true;
	}

	virtual void march(const CellPoint&) {}

	virtual bool progress()
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

	bool hasStat(const std::string& name) const
	{
		return _stats.find(name) != _stats.end();
	}

	OptStat getStatValue(const std::string& name) const
	{
		auto it = _stats.find(name);
		if (it != _stats.end())
		{
			return it->second.second;
		}

		return OptStat{};
	}

	void setStatValue(const std::string& name, statValue value)
	{
		auto it = _stats.find(name);
		if (it != _stats.end())
		{
			it->second.second = value;
		}
	}

	OptStat getStatBase(const std::string& name) const
	{
		auto it = _stats.find(name);
		if (it != _stats.end())
		{
			return it->second.first;
		}

		return OptStat{};
	}

	bool hasTrait(const std::string& name) const
	{
		return _traits.contains(name);
	}

	template <class AbilityType>
	bool hasAbility() const
	{
		return std::any_of(
			_abilities.begin(),
			_abilities.end(),
			[](const std::unique_ptr<IAbility>& ability)
			{ return dynamic_cast<AbilityType*>(ability.get()) != nullptr; });
	}

	template <class AbilityType>
	AbilityType* getAbility() const
	{
		AbilityType* result = nullptr;
		for (const auto& ability : _abilities)
		{
			result = dynamic_cast<AbilityType*>(ability.get());

			if (result)
			{
				break;
			}
		}

		return result;
	}

	Unit& addStat(const std::string& name, uint32_t value)
	{
		_stats[name] = {value, value};
		return *this;
	}

	Unit& addTrait(const std::string& trait)
	{
		_traits.emplace(trait);
		return *this;
	}

	template <class AbilityType, class... Args>
		requires std::derived_from<AbilityType, IAbility>
	Unit& addAbility(Args&&... args)
	{
		_abilities.emplace_back(std::make_unique<AbilityType>(std::forward<Args>(args)...));
		return *this;
	}

private:
	std::map<std::string, std::pair<statBaseValue, statValue>> _stats;
	std::vector<std::unique_ptr<IAbility>> _abilities;
	std::set<std::string> _traits;
	CellPoint _position;
	uint32_t _id;
};
