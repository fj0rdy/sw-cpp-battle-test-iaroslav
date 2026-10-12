#pragma once
#include <Core/System/CellPoint.hpp>
#include <Core/System/SimulationCommon.hpp>
#include <Core/Unit/Components/IAbility.hpp>
#include <Core/Unit/Unit.hpp>
#include <IO/Events/MarchEnded.hpp>
#include <IO/Events/MarchStarted.hpp>
#include <IO/Events/UnitMoved.hpp>

class Movement : public IAbility
{
public:
	explicit Movement(uint32_t range = 1) :
			IAbility(),
			_range(range)
	{}

	virtual void setTarget(Unit& user, const CellPoint& target)
	{
		_target = target;
		_hasTarget = true;

		logMarchStarted(user);
	}

	bool use(Unit& user) override
	{
		if (!_hasTarget)
		{
			return false;
		}

		CellPoint oldPosition(user.getPosition());
		if (_target != oldPosition)
		{
			auto map = SimulationCommon::MapManager();
			auto units = SimulationCommon::UnitManager();
			if (map && units)
			{
				CellPoint newPosition(oldPosition);
				CellPoint nextStep(oldPosition);
				for (uint32_t steps = 0; steps < _range; steps++)
				{
					map->advancePointToTarget(nextStep, _target);
					if (!map->isCellAvailible(nextStep) || !units->isCellAvailible(nextStep))
					{
						break;
					}

					newPosition = nextStep;
				}

				user.setPosition(newPosition);
				bool moved = newPosition != oldPosition;

				if (moved)
				{
					logUnitMoved(user);
				}

				if (_target == newPosition)
				{
					endMovement(user);
				}

				return moved;
			}
		}
		else
		{
			endMovement(user);
		}

		return false;
	}

protected:
	inline void endMovement(Unit& user)
	{
		_hasTarget = false;
		logMarchEnded(user);
	}

	inline void logMarchStarted(Unit& user)
	{
		SimulationCommon::LogEvent(
			sw::io::MarchStarted{user.getId(), user.getPosition().x, user.getPosition().y, _target.x, _target.y});
	}

	inline void logMarchEnded(Unit& user)
	{
		SimulationCommon::LogEvent(sw::io::MarchEnded{user.getId(), user.getPosition().x, user.getPosition().y});
	}

	inline void logUnitMoved(Unit& user)
	{
		SimulationCommon::LogEvent(sw::io::UnitMoved{user.getId(), user.getPosition().x, user.getPosition().y});
	}

protected:
	CellPoint _target;
	uint32_t _range;
	bool _hasTarget = false;
};
