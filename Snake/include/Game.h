#pragma once
#include <memory>
#include <Window.h>
#include <InputManager.h>
#include <SpriteRenderer.h>
#include <Snake.h>
#include <EntityManager.h>

using namespace cabrankengine;

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

		const uint16_t c_WindowWidth = 1280;
		const uint16_t c_WindowHeight = 720;
		const glm::vec2 c_SnakeSpriteSize = glm::vec2(40.0f, 40.0f);
		const std::string c_SpriteLabel = "sprite";
		const std::string c_SnakeLabel = "snake";
		const std::string c_BackgroundLabel = "background";

		GameState m_CurrentState;
		Window m_Window;
		InputManager m_InputManager;
		SpriteRenderer m_Renderer;
		EntityManager m_EntityManager;
	};
}

