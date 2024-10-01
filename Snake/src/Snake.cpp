#include <Snake.h>

using namespace snake;

Snake::Snake(vec2 pos, vec2 size, Texture2D sprite, vec3 color, vec2 velocity) : GameObject(pos, size, sprite, color, velocity), 
			 m_Tiles() 
{
	m_Tiles.emplace_back(pos, size, sprite, color, velocity);
}

void Snake::draw(SpriteRenderer& renderer)
{
	for (auto& tile : m_Tiles)
		tile.draw(renderer);
}

void Snake::reset()
{
	m_Tiles = std::vector<GameObject>(1);
}
