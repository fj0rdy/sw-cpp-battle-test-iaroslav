#pragma once
#include <Core/Components/IMapManager.hpp>
#include <Core/Components/IUnitManager.hpp>
#include <IO/System/TypeRegistry.hpp>

namespace SimulationCommon
{
	inline auto MapManager()
	{
		return sw::TypeRegistry::Common().get<IMapManager>();
	}

	inline auto UnitManager()
	{
		return sw::TypeRegistry::Common().get<IUnitManager>();
	}
}
