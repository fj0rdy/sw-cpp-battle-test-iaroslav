#pragma once
#include <Core/System/CellPoint.hpp>
#include <Core/Unit/Components/IAbility.hpp>
#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>

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

	inline uint32_t getId()
	{
		return _id;
	}

	virtual const std::string& getType() const;
	
    virtual const CellPoint& getPosition() const;
	virtual void setPosition(const CellPoint& position);

	virtual bool isAlive() const;
	virtual bool occupiesSpace() const;
	virtual void march(const CellPoint&) {}
	virtual bool progress();

	bool hasStat(const std::string& name) const;

	OptStat getStatValue(const std::string& name) const;
	void setStatValue(const std::string& name, statValue value);
    Unit& addStat(const std::string& name, uint32_t value);
	
    OptStat getStatBase(const std::string& name) const;
	bool hasTrait(const std::string& name) const;
	Unit& addTrait(const std::string& trait);

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