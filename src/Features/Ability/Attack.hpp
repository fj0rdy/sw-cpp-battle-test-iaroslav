#pragma once
#include <Core/Unit/Components/IAbility.hpp>
#include <cstdint>
#include <utility>

class Unit;

class Attack : public IAbility
{
public:
	Attack() = default;

	bool use(Unit& user) override;

protected:
	virtual bool canAttack(Unit&);
	virtual bool targetFilter(Unit&);
	virtual std::pair<uint32_t, uint32_t> getRange(Unit&);
	virtual uint32_t getDamage(Unit&, Unit&);

    void logAttack(Unit& user, Unit& target, uint32_t& damage, uint32_t& targetHp);
};
