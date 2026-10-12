#pragma once
#include <Core/System/details/TickCounter.hpp>
#include <IO/System/EventLog.hpp>
#include <IO/System/TypeRegistry.hpp>
#include <utility>

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

	inline thread_local TickCounter tickCounter{};
	inline thread_local sw::EventLog eventLog{};

	template <class EventT>
	void LogEvent(EventT&& event)
	{
		eventLog.log(tickCounter.get(), std::forward<EventT>(event));
	}
}
