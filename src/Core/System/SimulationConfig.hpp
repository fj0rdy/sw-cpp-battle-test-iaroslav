#pragma once
#include <cstdint>
#include <map>
#include <string>
#include <variant>
#include <concepts>

template<class T>
concept SimulationConfigValue = std::same_as<T, bool> || std::same_as<T, int> || 
    std::same_as<T, uint32_t> || std::same_as<T, double> || std::same_as<T, std::string>;

class SimulationConfig
{
public:
	using ValueVariant = std::variant<bool, int, uint32_t, double, std::string>;

	bool hasValue(const std::string& propertyName) const
	{
		return _values.find(propertyName) != _values.end();
	}

	// TODO: add concept to check that T is compatible with ValueVariant
	template <SimulationConfigValue T>
	void set(const std::string& propertyName, const T& value)
	{
		_values[propertyName] = value;
	}

	template <SimulationConfigValue T>
	bool read(T& to, const std::string& propertyName)
	{
		if (hasValue(propertyName) && std::holds_alternative<T>(_values[propertyName]))
		{
			to = std::get<T>(_values[propertyName]);
			return true;
		}
		return false;
	}

private:
	std::map<std::string, ValueVariant> _values;
};
