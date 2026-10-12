#include "Movement.hpp"

#include <Core/System/SimulationCommon.hpp>
#include <Core/Unit/Unit.hpp>
#include <IO/Events/MarchEnded.hpp>
#include <IO/Events/MarchStarted.hpp>
#include <IO/Events/UnitMoved.hpp>

namespace sw::sim
{

	Movement::Movement(Distance range /*= 1*/) :
			IAbility(),
			_range(range)
	{}

	void Movement::setTarget(Unit& user, const CellPoint& target)
	{
		_target = target;
		_hasTarget = true;

		logMarchStarted(user);
	}

	bool Movement::use(Unit& user)
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
				for (Distance steps = 0; steps < _range; steps++)
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

	void Movement::endMovement(Unit& user)
	{
		_hasTarget = false;
		logMarchEnded(user);
	}

	void Movement::logMarchStarted(Unit& user)
	{
		SimulationCommon::logEvent(
			io::MarchStarted{user.getId(), user.getPosition().x, user.getPosition().y, _target.x, _target.y});
	}

	void Movement::logMarchEnded(Unit& user)
	{
		SimulationCommon::logEvent(io::MarchEnded{user.getId(), user.getPosition().x, user.getPosition().y});
	}

	void Movement::logUnitMoved(Unit& user)
	{
		SimulationCommon::logEvent(io::UnitMoved{user.getId(), user.getPosition().x, user.getPosition().y});
	}

}  //namespace sw::sim
