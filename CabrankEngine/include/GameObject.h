#pragma once
#include <glm/glm.hpp>
#include <Texture2D.h>
#include <SpriteRenderer.h>

using namespace cabrankengine;
using namespace glm;

class GameObject
{
public:
	GameObject();
	GameObject(vec2 pos, vec2 size, Texture2D sprite, vec3 color = vec3(1.0f), vec2 velocity = vec2(0.0f, 0.0f));

	virtual void draw(SpriteRenderer& renderer);

	virtual void reset(vec2 position, vec2 size, vec2 velocity);

	// Getters and Setters
	vec2& GetPosition() { return m_Position; }
	
	const vec2& GetSize() const { return m_Size; }
	
	const vec2& GetVelocity() const { return m_Velocity; }
	
	void SetColor(vec3 newColor) { m_Color = newColor; }
	
	bool IsSolid() const { return m_Solid; }
	void SetSolid() { m_Solid = true; }
	
	bool IsDestroyed() const { return m_Destroyed; }
	void Destroy() { m_Destroyed = true; }

protected:
	vec2 m_Position, m_Size, m_Velocity;
	vec3 m_Color;
	float m_Rotation;
	bool m_Solid;
	bool m_Destroyed;

	Texture2D m_Sprite;
};

