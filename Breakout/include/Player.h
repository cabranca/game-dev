#pragma once

#include <GameObject.h>

namespace breakout {
	class Player : public cabrankengine::GameObject
	{
	public:
		Player(glm::vec2 pos, glm::vec2 size, cabrankengine::Texture2D sprite);

		void Extend(float value) { m_Size.x += value; }
	};
}
