#pragma once
#include <Core/Components/IMapManager.hpp>
#include <Core/Components/IUnitManager.hpp>
#include <IO/System/EventLog.hpp>
#include <IO/System/TypeRegistry.hpp>
#include <utility>

template <class C, class M, class U>
class Simulation;

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

	class TickCounter
	{
		template <class C, class M, class U>
		friend class ::Simulation;

	public:
		uint64_t get()
		{
			return _tickCount;
		}

	protected:
		void set(uint64_t value)
		{
			_tickCount = value;
		}

		void reset()
		{
			_tickCount = 0;
		}

	private:
		uint64_t _tickCount = 0;
	};

	inline thread_local TickCounter tickCounter{};
	inline thread_local sw::EventLog eventLog{};

	template <class EventT>
	void LogEvent(EventT&& event)
	{
		eventLog.log(tickCounter.get(), std::forward<EventT>(event));
	}
}
