#pragma once
#include <cstdint>

namespace snake {
	class Game {
	public:
		Game(uint16_t windowWidth, uint16_t windowHeight);
		~Game();

		void init();
		void processInput(float delta);
		void update(float delta);
		void render();

	private:
		uint16_t m_WindowWidth;
		uint16_t m_WindowHeight;
	};
}

