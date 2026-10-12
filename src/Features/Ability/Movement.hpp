#pragma once
#include <Core/System/CellPoint.hpp>
#include <Core/Unit/Components/IAbility.hpp>

class Unit;

class Movement : public IAbility
{
public:
	explicit Movement(uint32_t range = 1);

	virtual void setTarget(Unit& user, const CellPoint& target);
	bool use(Unit& user) override;

protected:
	inline void endMovement(Unit& user);

	inline void logMarchStarted(Unit& user);
	inline void logMarchEnded(Unit& user);
	inline void logUnitMoved(Unit& user);

protected:
	CellPoint _target;
	uint32_t _range;
	bool _hasTarget = false;
};
