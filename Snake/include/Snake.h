#pragma once
#include <GameObject.h>
#include <vector>

using namespace cabrankengine;

namespace snake
{
	class Snake : public GameObject
	{
	public:
		Snake(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));

		void draw(SpriteRenderer& renderer);

		void reset();

	private:
		std::vector<GameObject> m_Tiles;
	};
}
