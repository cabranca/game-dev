#include <GameLevel.h>
#include <ResourceManager.h>
#include <fstream>
#include <sstream>

using namespace breakout;

GameLevel::GameLevel()
{
}

void GameLevel::Load(const char* file, unsigned int levelWidth, unsigned int levelHeight)
{
	m_Bricks.clear();
	unsigned int tileCode;
	GameLevel level;
	std::string line;
	std::ifstream fstream(file);
	std::vector<std::vector<unsigned int>> tileData;

	if (fstream) {
		while (std::getline(fstream, line)) {
			std::istringstream sstream(line);
			std::vector<unsigned int> row;
			while (sstream >> tileCode)
				row.push_back(tileCode);
			tileData.push_back(row);
		}

		if (tileData.size() > 0)
			init(tileData, levelWidth, levelHeight);
	}
}

void GameLevel::Draw(SpriteRenderer& renderer)
{
	// Could be changed to a system where the destroyed tiles are removed from the array
	for (auto& tile : m_Bricks)
		if (!tile.IsDestroyed())
			tile.draw(renderer);
}

bool GameLevel::IsCompleted()
{
	for (auto& tile : m_Bricks)
		if (!tile.IsSolid() && !tile.IsDestroyed())
			return false;
	return true;
}

void GameLevel::init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight)
{
	// Calculate dimensions
	unsigned int height = tileData.size();
	unsigned int width = tileData[0].size();
	float unit_width = levelWidth / static_cast<float>(width);
	float unit_height = levelHeight / static_cast<float>(height);

	// Initialize level tiles based on tileData
	for (unsigned int y = 0; y < height; y++) {
		for (unsigned int x = 0; x < width; x++) {
			// Check block type from level data (2D level array)
			if (tileData[y][x] == 1) { // solid
				vec2 pos(unit_width * x, unit_height * y);
				vec2 size(unit_width, unit_height);
				GameObject obj(pos, size, ResourceManager::getTexture("block_solid"), vec3(0.8f, 0.8f, 0.7f));
				obj.SetSolid();
				m_Bricks.push_back(obj);
			}
			else if (tileData[y][x] > 1) {
				vec3 color = vec3(1.0f); // original: white
				if (tileData[y][x] == 2)
					color = vec3(0.2f, 0.6f, 1.0f);
				else if (tileData[y][x] == 3)
					color = vec3(0.0f, 0.7f, 0.0f);
				else if (tileData[y][x] == 4)
					color = vec3(0.8f, 0.8f, 0.4f);
				else if (tileData[y][x] == 5)
					color = vec3(1.0f, 0.5f, 0.0f);

				vec2 pos(unit_width * x, unit_height * y);
				vec2 size(unit_width, unit_height);
				this->m_Bricks.push_back(
					GameObject(pos, size, ResourceManager::getTexture("block"), color)
				);
			}
		}
	}
}
