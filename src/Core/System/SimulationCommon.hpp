#pragma once

#include <Core/System/details/TickCounter.hpp>
#include <IO/System/EventLog.hpp>
#include <IO/System/PrintDebug.hpp>
#include <IO/System/TypeRegistry.hpp>
#include <iostream>
#include <utility>

namespace sw::sim
{

	namespace SimulationCommon
	{
		inline auto MapManager()
		{
			return TypeRegistry::Common().get<IMapManager>();
		}

		inline auto UnitManager()
		{
			return TypeRegistry::Common().get<IUnitManager>();
		}

		inline thread_local TickCounter tickCounter{};
		inline thread_local EventLog eventLog{};

		template <class EventT>
		void logEvent(EventT&& event)
		{
			eventLog.log(tickCounter.get(), std::forward<EventT>(event));
		}

		inline void logWarning(const std::string& warning)
		{
			std::cerr << warning;
		}
	}

}  //namespace sw::sim
