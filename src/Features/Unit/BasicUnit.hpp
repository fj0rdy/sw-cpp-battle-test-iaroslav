#pragma once
#include <Core/Unit/Unit.hpp>

class BasicUnit : public Unit
{
public:
	BasicUnit(uint32_t id) :
			Unit(id)
	{}

protected:
	const std::string& getType() const override;
	bool isAlive() const override;
	bool occupiesSpace() const override;
	void march(const CellPoint& point) override;
};
