#pragma once
#include <glm/glm.hpp>

namespace cabrankengine
{
	// TODO: Convert this to a struct without a base. Check the "has" inside EntityManager.
	class Component
	{
	public:
		bool has = false;
	};

	class CPlayer : public Component {};

	class CTransform : public Component
	{
	public:
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;

		CTransform() : position(), rotation(), scale(0.f) {}

		CTransform(const glm::vec3& p, const glm::vec3& r, const glm::vec3& s)
			: position(p), rotation(r), scale(s) {}
	};

	class CBoundingBox : public Component
	{
	public:
		glm::vec2 size;
		glm::vec2 halfSize;

		CBoundingBox() : size(1.f, 1.f), halfSize(0.5f, 0.5f) {}

		CBoundingBox(const glm::vec2& s)
			: size(s), halfSize(s.x / 2, s.y / 2) {}
	};

	class CLinearMovement : public Component
	{
	public:
		glm::vec2 direction;
		float speed;
		CLinearMovement() : direction(0.f, 0.f), speed(0.f) {}
		CLinearMovement(const glm::vec2& dir, float s) : direction(dir), speed(s) {}
	};
}
