#pragma once

#include <Core/System/CellPoint.hpp>
#include <Core/System/types.hpp>
#include <Core/Unit/Components/IAbility.hpp>
#include <algorithm>
#include <concepts>
#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

namespace sw::sim
{

	class Unit
	{
	public:
		using Ptr = std::shared_ptr<Unit>;
		using StatBase = Stat;
		using OptStat = std::optional<Stat>;

	public:
		explicit Unit(Id id) :
				_id(id)
		{}

		virtual ~Unit() = default;

		inline Id getId()
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
		OptStat getStatBase(const std::string& name) const;
		virtual void setStatValue(const std::string& name, Stat value);
		virtual void setStatBase(const std::string& name, Stat baseValue);
		Unit& addStat(const std::string& name, Stat value);

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
		std::map<std::string, std::pair<StatBase, Stat>> _stats;
		std::vector<std::unique_ptr<IAbility>> _abilities;
		std::set<std::string> _traits;
		CellPoint _position;
		Id _id;
	};

}  //namespace sw::sim
