#pragma once

#include <Core/Components/ICommandSource.hpp>
#include <Core/Components/IMapManager.hpp>
#include <Core/Components/IUnitManager.hpp>
#include <concepts>

namespace sw::sim
{

	template <class CT, class MT, class UT>
	concept SimulationComponents = std::derived_from<CT, ICommandSource> && std::derived_from<MT, IMapManager>
								   && std::derived_from<UT, IUnitManager>;

}  //namespace sw::sim
