#pragma once

#include <cstdint>

namespace sw::sim
{

	using Stat = uint32_t;
	using Distance = uint32_t;	// in general should not be narrower that Stat
	using Id = uint32_t;
	using Tick = uint64_t;

}  //namespace sw::sim
