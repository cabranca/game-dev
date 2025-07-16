#pragma once
#include <vector>
#include <GameObject.h>

namespace breakout {
	class GameLevel
	{
	public:
		GameLevel();
		void Load(const char* file, unsigned int levelWidth, unsigned int levelHeight);
		void Draw(cabrankengine::SpriteRenderer& renderer);
		bool IsCompleted();

		std::vector<cabrankengine::GameObject>& GetBricks() { return m_Bricks; }

	private:
		std::vector<cabrankengine::GameObject> m_Bricks;

		void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);
	};
}
