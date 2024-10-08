#include <CollisionSolver.h>

using namespace cabrankengine;

bool CollisionSolver::AABBCollision(EntPtr b1, EntPtr b2)
{
	auto& pos1 = b1->getComponent<CTransform>().pos;
	auto& pos2 = b2->getComponent<CTransform>().pos;
	auto& half1 = b1->getComponent<CBoundingBox>().halfSize;
	auto& half2 = b2->getComponent<CBoundingBox>().halfSize;

	auto [deltax, deltay] = std::make_pair(abs(pos1.x - pos2.x), abs(pos1.y - pos2.y));
	auto overlapx = half1.x + half2.x - deltax;
	auto overlapy = half1.y + half2.y - deltay;
	return overlapx >= 0 && overlapy >= 0;
}