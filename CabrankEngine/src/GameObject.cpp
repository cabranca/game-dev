#include <GameObject.h>

using namespace cabrankengine;
using namespace glm;

GameObject::GameObject()
	: m_Position(0.0f, 0.0f), m_Size(1.0f, 1.0f), m_Velocity(0.0f), m_Color(1.0f), m_Rotation(0.0f), m_Sprite(), m_Solid(false), m_Destroyed(false) { }

GameObject::GameObject(vec2 pos, vec2 size, Texture2D sprite, vec3 color, vec2 velocity)
	: m_Position(pos), m_Size(size), m_Velocity(velocity), m_Color(color), m_Rotation(0.0f), m_Sprite(sprite), m_Solid(false), m_Destroyed(false) { }

void GameObject::draw(SpriteRenderer& renderer) {
	renderer.drawSprite(m_Sprite, m_Position, m_Size, m_Rotation, m_Color);
}

void GameObject::reset(vec2 position, vec2 size, vec2 velocity)
{
	m_Position = position;
	m_Size = size;
	m_Velocity = velocity;
}
