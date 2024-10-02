#pragma once

#include <GameObject.h>

namespace breakout {
	class Player : public GameObject
	{
	public:
		Player(glm::vec2 pos, glm::vec2 size, Texture2D sprite);

		void Extend(float value) { m_Size.x += value; }
	};
}
