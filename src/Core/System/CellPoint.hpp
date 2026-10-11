#pragma once
#include <cstdint>

struct CellPoint
{
	CellPoint(uint32_t xv = 0, uint32_t yv = 0) :
			x(xv),
			y(yv)
	{}

	bool operator==(const CellPoint&) const = default;

	uint32_t x;
	uint32_t y;
};
