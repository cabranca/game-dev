#pragma once
#include <GameObject.h>
#include <vector>

using namespace cabrankengine;

namespace snake
{
	class Snake : public GameObject
	{
	public:
		Snake(vec2 pos, vec2 size, Texture2D sprite, vec3 color = vec3(1.0f), vec2 velocity = vec2(0.0f, 0.0f));

		void draw(SpriteRenderer& renderer);

		void reset();

	private:
		std::vector<GameObject> m_Tiles;
	};
}
