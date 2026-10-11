#pragma once

#include <Core/System/CellPoint.hpp>
#include <Core/Unit/Components/IAbility.hpp>
#include <algorithm>
#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

class Unit
{
public:
	using UnitPtr = std::shared_ptr<Unit>;
	using statType = uint32_t;
	using OptStat = std::optional<statType>;

public:
	explicit Unit(uint32_t id) :
			_id(id)
	{}

	virtual ~Unit() = default;

	void setId(uint32_t id)
	{
		_id = id;
	}

	uint32_t getId()
	{
		return _id;
	}

	virtual const CellPoint& getPosition() const
	{
		return _position;
	}

	virtual void setPosition(const CellPoint& p)
	{
		_position = p;
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
			if (ability && ability->use(this))
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

	OptStat getStat(const std::string& name) const
	{
		auto it = _stats.find(name);
		if (it != _stats.end())
		{
			return it->second;
		}

		return OptStat{};
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

	Unit& operator<<(std::pair<const std::string, uint32_t> stat)
	{
		_stats[stat.first] = stat.second;
		return *this;
	}

	Unit& operator<<(std::unique_ptr<IAbility> ability)
	{
		if (ability)
		{
			_abilities.emplace_back(std::move(ability));
		}
		return *this;
	}

private:
	std::map<std::string, statType> _stats;
	std::vector<std::unique_ptr<IAbility>> _abilities;
	CellPoint _position;
	uint32_t _id;
};
