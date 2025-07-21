#pragma once
#include <glm/glm.hpp>
#include <string>

namespace cabrankengine
{
	class Component
	{
	public:
		bool has = false;
	};

	class CPlayer : public Component {};

	class CEnemy : public Component {};

	class CTransform : public Component
	{
	public:
		glm::vec2 pos;
		glm::vec2 velocity;
		float angle;

		CTransform() : pos(), velocity(), angle(0.f) {}

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

		CTexture() : name(""), pos(0.f, 0.f), size(1.f, 1.f), color(1.f, 1.f, 1.f) {}

		CTexture(const std::string& n, const glm::vec2& p, const glm::vec2& s, const glm::vec3& c)
			: name(n), pos(p), size(s), color(c) {}
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

	class CInput : public Component
	{
	public:
		bool up;
		bool down;
		bool right;
		bool left;
		bool shoot;

		CInput() : up(false), down(false), right(false), left(false), shoot(false) {}
	};

	class CLinearMovement : public Component
	{
	public:
		glm::vec2 direction;
		float speed;
		CLinearMovement() : direction(0.f, 0.f), speed(0.f) {}
		CLinearMovement(const glm::vec2& dir, float s) : direction(dir), speed(s) {}
	};

	class CHealth : public Component
	{
	public:
		int maxHealth;
		int currentHealth;
		CHealth() : maxHealth(100), currentHealth(100) {}
		CHealth(int maxH, int currentH) : maxHealth(maxH), currentHealth(currentH) {}
	};

	class CDamage : public Component
	{
	public:
		int damageAmount;
		CDamage() : damageAmount(10) {}
		CDamage(int dmg) : damageAmount(dmg) {}
	};

	class CScore : public Component
	{
	public:
		int score;

		CScore() : score(0) {}

		CScore(int s) : score(s) {}
	};
}