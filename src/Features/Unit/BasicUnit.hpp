#pragma once

#include <Core/System/types.hpp>
#include <Core/Unit/Unit.hpp>

namespace sw::sim
{

	class BasicUnit : public Unit
	{
	public:
		BasicUnit(Id id) :
				Unit(id)
		{}

	protected:
		const std::string& getType() const override;
		bool isAlive() const override;
		bool occupiesSpace() const override;
		void march(const CellPoint& point) override;
	};

}  //namespace sw::sim
