#pragma once

#include <Core/System/types.hpp>

namespace sw::sim
{

	struct CellPoint
	{
		CellPoint(Distance xv = 0, Distance yv = 0) :
				x(xv),
				y(yv)
		{}

		bool operator==(const CellPoint&) const = default;

		Distance x;
		Distance y;
	};

}  //namespace sw::sim
