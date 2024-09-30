#pragma once
#include <memory>
#include <SpriteRenderer.h>
#include <vector>
#include <GameLevel.h>
#include <Ball.h>
#include <ParticleGenerator.h>
#include <PostProcessor.h>
#include <PowerUp.h>
#include <irrKlang/irrKlang.h>
#include <TextRenderer.h>
#include <Player.h>
#include <InputManager.h>

namespace breakout {
	// Represents the current state of the game
	enum GameState {
		GAME_ACTIVE,
		GAME_MENU,
		GAME_WIN,
		GAME_QUIT
	};

	// Represent vector directions for collsion detections.
	enum Direction {
		UP,
		RIGHT,
		DOWN,
		LEFT
	};

	typedef std::tuple<bool, Direction, glm::vec2> Collision;

	class Game
	{
	public:
		GameState State;

		Game(unsigned int width, unsigned int height);
		~Game();

		// Initialize game state (load all shaders/textures/levels)
		void Init();

		// Game loop
		void ProcessInput(float delta);

		void Update(float delta);

		void Render();

		void DoCollisions();

		// reset
		void ResetLevel();
		void ResetPlayer();

		// powerups
		void SpawnPowerUps(GameObject& block);
		void UpdatePowerUps(float dt);

		// Getters and Setters
		void SetKey(int key, bool value) { m_Keys[key] = value; }
		void ReleaseKey(int key) { m_KeysProcessed[key] = false; }

	private:
		bool m_Keys[1024];
		bool m_KeysProcessed[1024];
		unsigned int m_Width, m_Height;
		std::vector<GameLevel> m_Levels;
		unsigned int m_CurrentLevel;
		unsigned int m_Lives;

		std::unique_ptr<irrklang::ISoundEngine> m_SoundEngine;
		std::unique_ptr<SpriteRenderer> m_Renderer;
		std::unique_ptr<TextRenderer> m_TextRenderer;
		std::unique_ptr<Player> m_Player;
		std::unique_ptr<Ball> m_Ball;
		std::unique_ptr<ParticleGenerator> m_Particles;
		std::vector<std::shared_ptr<PowerUp>> m_PowerUps;
		std::unique_ptr<PostProcessor> m_Effects;
		std::unique_ptr<InputManager> m_InputManager;
		float m_ShakeTime = 0.0f;

		const glm::vec2 c_PlayerSize = glm::vec2(100.0f, 20.0f);
		const float c_PlayerVelocity = 300.0f;
		const glm::vec2 c_InitialBallVelocity = glm::vec2(100.0f, -350.0f);
		const float c_BallRadius = 12.5f;

		// AABB - AABB collision
		static bool checkCollision(GameObject& one, GameObject& two);
		// AABB - Circle collision
		static Collision checkCollision(Ball& one, GameObject& two);
		static Direction vectorDirection(glm::vec2 target);

		// powerups
		bool IsOtherPowerUpActive(std::vector<std::shared_ptr<PowerUp>>& powerUps, std::string type);
		bool ShouldSpawn(unsigned int chance);
		void ActivatePowerUp(PowerUp& powerUp);
	};
}
