#pragma once
#include <memory>
#include <Window.h>
#include <InputManager.h>
#include <SpriteRenderer.h>
#include <Snake.h>

using namespace cabrankengine;
using namespace glm;

namespace snake {
	class Game {
	public:

		Game();

		void run();

	private:

		enum GameState { ACTIVE, QUIT };

		void initSystems();
		void loadShaders();
		void processInput();
		void update(float delta);
		void render();

		const uint16_t c_WindowWidth = 800;
		const uint16_t c_WindowHeight = 600;
		const vec2 c_SnakeSpriteSize = vec2(100.0f, 100.0f);
		const std::string c_SpriteLabel = "sprite";
		const std::string c_SnakeLabel = "snake";
		const std::string c_BackgroundLabel = "background";

		GameState m_CurrentState;
		std::unique_ptr<Window> m_Window;
		std::unique_ptr<InputManager> m_InputManager;
		std::unique_ptr<SpriteRenderer> m_Renderer;
		std::unique_ptr<Snake> m_Snake;
	};
}

