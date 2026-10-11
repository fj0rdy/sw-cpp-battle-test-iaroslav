#pragma once
#include <Core/Components/ICommandSource.hpp>
#include <Core/System/SimulationConfig.hpp>
#include <fstream>
#include <stdexcept>
#include <string>

class FileCommandSource : public ICommandSource
{
public:
	FileCommandSource(SimulationConfig& config) :
			ICommandSource(config)
	{
		if (!config.read(_filePath, "commandFilePath"))
		{
			throw std::runtime_error{"command file is not specified!"};
		}
	}

	bool executeAll() override
	{
		bool result = false;
		if (!_executed)
		{
			std::ifstream file(_filePath);
			if (!file)
			{
				throw std::runtime_error("Error: File not found - " + _filePath);
			}

			_parser.parse(file);
			result = _executed = true;
		}

		return result;
	}

private:
	std::string _filePath;
	bool _executed = false;
};
