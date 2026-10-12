#include "IO/System/TypeRegistry.hpp"

#include <Core/Simulation.hpp>
#include <Core/System/SimulationConfig.hpp>
#include <Features/Components/BasicMapManager.hpp>
#include <Features/Components/BasicUnitManager.hpp>
#include <Features/Components/FileCommandSource.hpp>
#include <Features/Unit/Hunter.hpp>
#include <Features/Unit/Swordsman.hpp>
#include <IO/System/EventLog.hpp>
#include <IO/System/PrintDebug.hpp>
#include <fstream>
#include <iostream>
#include <stdexcept>

int main(int argc, char** argv)
{
	using namespace sw::sim;

	if (argc != 2)
	{
		throw std::runtime_error("Error: No file specified in command line argument");
	}

	SimulationConfig config;
	config.set("commandFilePath", std::string(argv[1]));

	using Unitmanager = BasicUnitmanager<
		Swordsman,
		Hunter
		/*insert new unit types here*/>;

	Simulation<FileCommandSource, BasicMapManager, Unitmanager> simulation(config);
	simulation.start();

	return 0;
}
