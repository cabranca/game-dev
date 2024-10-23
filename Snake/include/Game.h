#pragma once
#include <memory>
#include <Window.h>
#include <InputManager.h>
#include <SpriteRenderer.h>
#include <Snake.h>
#include <EntityManager.h>
#include <queue>

using namespace cabrankengine;

namespace snake {
	class Game {
	public:

		Game();

		void run();

	private:

		enum GameState { ACTIVE, QUIT };

		void init();
		void loadShaders();
		void loadTextures();
		void update();
		void sUserInput();
		void sMovement();
		void sCollision();
		void sRender();

		void setBackground();
		void spawnSnake();
		void spawnFruit();
		void increaseTail();

		const int c_WindowWidth = 800;
		const int c_WindowHeight = 800;
		const glm::vec2 c_SnakeSpriteSize = glm::vec2(40.0f, 40.0f);
		const glm::vec2 c_FruitSpriteSize = glm::vec2(40.0f, 40.0f);
		const std::string c_SpriteLabel = "sprite";
		const std::string c_SnakeLabel = "snake";
		const std::string c_FruitLabel = "fruit";
		const std::string c_BackgroundLabel = "background";
		const float c_SnakeSpeed = 40.0f;

		GameState m_CurrentState;
		Window m_Window;
		InputManager m_InputManager;
		std::unique_ptr<SpriteRenderer> m_Renderer; // I still do not get why it only works with a pointer.
		EntityManager m_EntityManager;

		EntPtr m_Head;
		EntPtr m_Tail;
		std::vector<std::queue<unsigned int>> m_RegisteredInputs;
		unsigned int m_LastInput;
	};
}

