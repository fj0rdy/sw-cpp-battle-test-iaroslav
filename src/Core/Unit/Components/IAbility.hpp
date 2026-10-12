#pragma once

class Unit;

class IAbility
{
public:
	IAbility() = default;
	virtual ~IAbility() = default;
	virtual bool use(Unit& user) = 0;
};
