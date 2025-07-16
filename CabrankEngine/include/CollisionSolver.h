#pragma once
#include <Entity.h>
#include <memory>

namespace cabrankengine
{
	using EntPtr = std::shared_ptr<Entity>;
	class CollisionSolver
	{
	public:
		static bool AABBCollision(EntPtr b1, EntPtr b2);
	};
}