#pragma once

#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <SpriteRenderer.h>
#include <TextRenderer.h>
#include <EntityManager.h>
#include <Window.h>
#include <InputManager.h>

namespace ss
{
	constexpr float WINDOW_WIDTH = 1280.f;
	constexpr float WINDOW_HEIGHT = 720.f;
	constexpr glm::vec2 PLAYER_SPRITE_SIZE = glm::vec2(80.0f, 80.0f);
	constexpr glm::vec2 ENEMY_SPRITE_SIZE = glm::vec2(80.0f, 80.0f);
	constexpr glm::vec2 BULLET_SPRITE_SIZE = glm::vec2(20.0f, 20.0f);
	constexpr std::string SPRITE_LABEL = "sprite";
	constexpr std::string PLAYER_LABEL = "player";
	constexpr std::string ENEMY_LABEL = "enemy";
	constexpr std::string BULLET_LABEL = "bullet";
	constexpr std::string BACKGROUND_LABEL = "background";
	constexpr float PLAYER_SPEED = 40.0f;
	constexpr float ENEMY_SPEED = 30.0f;
	constexpr float BULLET_SPEED = 60.0f;

	class Game
	{
	public:
		Game();

		void run();

	private:
		enum GameState { ACTIVE, LOST, QUIT };

		void init();
		void loadShaders();
		void loadTextures();
		void update(float deltaTime);
		void sUserInput();
		void sMovement();
		void sCollision();
		void sRender();

		GameState m_CurrentState;
		cabrankengine::Window m_Window;
		cabrankengine::InputManager m_InputManager;
		std::unique_ptr<cabrankengine::SpriteRenderer> m_Renderer; // I still do not get why it only works with a pointer.
		std::unique_ptr<cabrankengine::TextRenderer> m_TextRenderer;
		cabrankengine::EntityManager m_EntityManager;
	};
}
