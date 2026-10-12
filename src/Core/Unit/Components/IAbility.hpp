#pragma once

namespace sw::sim
{

	class Unit;

	class IAbility
	{
	public:
		IAbility() = default;
		virtual ~IAbility() = default;
		virtual bool use(Unit& user) = 0;
	};

}  //namespace sw::sim
