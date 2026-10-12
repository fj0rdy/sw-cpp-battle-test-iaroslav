#pragma once

#include <Core/System/CellPoint.hpp>
#include <Core/Unit/Components/IAbility.hpp>

namespace sw::sim
{

	class Unit;

	class Movement : public IAbility
	{
	public:
		explicit Movement(Distance range = 1);

		virtual void setTarget(Unit& user, const CellPoint& target);
		bool use(Unit& user) override;

	protected:
		void endMovement(Unit& user);

		void logMarchStarted(Unit& user);
		void logMarchEnded(Unit& user);
		void logUnitMoved(Unit& user);

	protected:
		CellPoint _target;
		Distance _range;
		bool _hasTarget = false;
	};

}  //namespace sw::sim
