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

		CTransform() = default;

		CTransform(const glm::vec2& p, const glm::vec2& v, float a)
			: pos(p), velocity(v), angle(a) {}
	};

	class CTexture : public Component
	{
	public:
		std::string name;
		glm::vec2 pos;
		glm::vec2 size;
		glm::vec3 color;

		CTexture() = default;

		CTexture(const std::string& n, const glm::vec2& p, const glm::vec2& s, const glm::vec3& c)
			: name(n), pos(p), size(s), color(c) {}
	};

	class CBoundingBox : public Component
	{
	public:
		glm::vec2 size;
		glm::vec2 halfSize;

		CBoundingBox() = default;

		CBoundingBox(const glm::vec2& s)
			: size(s), halfSize(s.x / 2, s.y / 2) {}
	};

	class CInput : public Component
	{
	public:
		bool up;
		bool down;
		bool right;
		bool left;

		CInput() = default;

		CInput(bool u, bool d, bool r, bool l)
			: up(u), down(d), right(r), left(l) {}
	};

	class CScore : public Component
	{
	public:
		int score;

		CScore() = default;

		CScore(int s) : score(s) {}
	};
}