#pragma once
#include <cstdint>
#include <map>
#include <string>
#include <variant>

class SimulationConfig
{
public:
	using ValueVariant = std::variant<bool, uint32_t, double, std::string>;

	bool hasValue(const std::string& propertyName) const
	{
		return _values.find(propertyName) != _values.end();
	}

	// TODO: add concept to check that T is compatible with ValueVariant
	template <class T>
	void set(const std::string& propertyName, const T& value)
	{
		_values[propertyName] = value;
	}

	template <class T>
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
