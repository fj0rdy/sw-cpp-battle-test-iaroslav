#pragma once
#include <IO/System/CommandParser.hpp>

class SimulationConfig;

class ICommandSource
{
public:
	ICommandSource(SimulationConfig&) {}

	virtual ~ICommandSource() = default;

	template <class TCommandData>
	void add(std::function<void(TCommandData)> handler)
	{
		_parser.add<TCommandData>(std::move(handler));
	}

	virtual bool executeAll() = 0;

protected:
	sw::io::CommandParser _parser;
};
