#include <Ball.h>

using namespace breakout;

Ball::Ball() : GameObject(), c_Radius(0), c_InitialVelocity(), m_Stuck(true), m_Sticky(false), m_PassThrough(false) { }

Ball::Ball(vec2 pos, float radius, vec2 velocity, Texture2D sprite)
	: GameObject(pos, vec2(radius * 2.0f, radius * 2.0f), sprite, vec3(1.0f), velocity),
	c_Radius(radius), c_InitialVelocity(velocity), m_Stuck(true), m_Sticky(false), m_PassThrough(false) { }

vec2 Ball::Move(float delta, unsigned int window_width)
{
	if (m_Stuck)
		return m_Position;

	m_Position += m_Velocity * delta;

	// Clamp and bounce if it reaches a wall
	if (m_Position.x <= 0.0f) {
		m_Velocity.x = -m_Velocity.x;
		m_Position.x = 0.0f;
	}
	else if (m_Position.x + m_Size.x >= window_width) {
		m_Velocity.x = -m_Velocity.x;
		m_Position.x = window_width - m_Size.x;
	}
	if (m_Position.y <= 0.0f) {
		m_Velocity.y = -m_Velocity.y;
		m_Position.y = 0.0f;
	}

	return m_Position;
}

void Ball::Reset(vec2 position, vec2 size, vec2 velocity)
{
	GameObject::Reset(position, size, velocity);
	m_Stuck = true;
	m_Sticky = false;
	m_PassThrough = false;
}

void Ball::BounceToWall(Axis axis)
{
	switch (axis) {
	case H:
		m_Velocity.x = -m_Velocity.x;
		break;
	case V:
		m_Velocity.y = -m_Velocity.y;
		break;
	}
}

void Ball::Accelerate(float factor)
{
	m_Velocity *= factor;
}

void Ball::BounceToPlayer(float percentage)
{
	vec2 oldVelocity = m_Velocity;
	m_Velocity.x = c_InitialVelocity.x * percentage * c_Strength;
	m_Velocity = normalize(m_Velocity) * length(oldVelocity); // keep speed consistent over both axes (multiply by length of old velocity, so total strength is not changed)
	// fix sticky paddle
	m_Velocity.y = -1.0f * abs(m_Velocity.y);
}
