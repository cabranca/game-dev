#pragma once
#include <glm/glm.hpp>
#include <Texture2D.h>
#include <SpriteRenderer.h>

using namespace cabrankengine;

class GameObject
{
public:
	GameObject();
	GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));

	virtual void draw(SpriteRenderer& renderer);

	virtual void reset(glm::vec2 position, glm::vec2 size, glm::vec2 velocity);

	// Getters and Setters
	glm::vec2& GetPosition() { return m_Position; }
	
	const glm::vec2& GetSize() const { return m_Size; }
	
	const glm::vec2& GetVelocity() const { return m_Velocity; }
	
	void SetColor(glm::vec3 newColor) { m_Color = newColor; }
	
	bool IsSolid() const { return m_Solid; }
	void SetSolid() { m_Solid = true; }
	
	bool IsDestroyed() const { return m_Destroyed; }
	void Destroy() { m_Destroyed = true; }

protected:
	glm::vec2 m_Position, m_Size, m_Velocity;
	glm::vec3 m_Color;
	float m_Rotation;
	bool m_Solid;
	bool m_Destroyed;

	Texture2D m_Sprite;
};

