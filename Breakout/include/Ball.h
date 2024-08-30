#pragma once
#include <GameObject.h>

namespace breakout {
	enum Axis { H, V };

	class Ball : public GameObject
	{
	public:
		Ball();
		Ball(glm::vec2 pos, float radius, glm::vec2 initialVelocity, Texture2D sprite);

		glm::vec2 Move(float delta, unsigned int window_width);
		void Reset(glm::vec2 position, glm::vec2 size, glm::vec2 velocity) override;
		void BounceToWall(Axis direction);
		void Accelerate(float factor);
		void BounceToPlayer(float percentage);

		// Getters and Setters
		float GetRadius() const { return c_Radius; }
		bool IsStuck() const { return m_Stuck; }
		void SetStuck(bool newValue) { m_Stuck = newValue; }
		bool IsSticky() const { return m_Sticky; }
		void SetSticky(bool newValue) { m_Sticky = newValue; }
		void SetPassThrough(bool newValue) { m_PassThrough = newValue; }

	private:
		bool m_Stuck;
		bool m_Sticky;
		bool m_PassThrough;

		const float c_Strength = 2.0f;
		const float c_Radius;
		const glm::vec2 c_InitialVelocity;
	};
}
