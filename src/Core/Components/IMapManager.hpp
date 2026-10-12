#pragma once

#include <Core/System/CellPoint.hpp>
#include <Core/System/types.hpp>
#include <cstdint>
#include <memory>

namespace sw::sim
{

	class SimulationConfig;
	class ICommandSource;

	class IMapManager
	{
	public:
		IMapManager(SimulationConfig&) {}

		virtual ~IMapManager() = default;

		bool isInitialized()
		{
			return _width && _height;
		}

		virtual void registerCommandHandlers(std::shared_ptr<ICommandSource>) = 0;
		virtual void progress() = 0;

		virtual bool isCellValid(const CellPoint&) = 0;
		virtual bool isCellAvailible(const CellPoint&) = 0;
		virtual Distance distance(const CellPoint&, const CellPoint&) = 0;
		virtual void clampPoint(CellPoint&) = 0;
		virtual void advancePointToTarget(CellPoint&, const CellPoint&) = 0;

	protected:
		void resize(Distance width, Distance height)
		{
			_width = width;
			_height = height;
		}

	protected:
		Distance _width = 0;
		Distance _height = 0;
	};

}  //namespace sw::sim
