#pragma once
#include <glm/glm.hpp>

namespace cabrankengine
{
	class Component
	{
	public:
		bool has = false;
	};

	class CTransform : public Component
	{
	public:
		glm::vec2 pos;
		glm::vec2 velocity;
		float angle;

		CTransform(const glm::vec2& p, const glm::vec2& v, float a)
			: pos(p), velocity(v), angle(a) {}
	};

	class CScore : public Component
	{
	public:
		int score;

		CScore(int s) : score(s) {}
	};
}