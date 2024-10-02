#include <iostream>
#include <CabrankEngine.h>
#include <Window.h>
#include <GL/glew.h>
#include <Game.h>

//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

constexpr unsigned int SCREEN_WIDTH = 800;
constexpr unsigned int SCREEN_HEIGHT = 600;

//breakout::Game breakoutGame(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char** argv) {
	cabrankengine::CabrankEngine::init();

	cabrankengine::Window window;
	window.create("Breakout", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

	breakout::Game game(SCREEN_WIDTH, SCREEN_HEIGHT);
	game.Init();

	// deltaTime variables
	const float DESIRED_FRAME_TIME = 1.0f / 60.0f;
	float previousTicks = 0.0f;
	float frameTime = 0.0f;

	// Window loop
	while (game.State != breakout::GameState::GAME_QUIT)
	{
		// Calculate delta time
		float newTicks = static_cast<float>(SDL_GetTicks()) / 1000.0f;
		frameTime = newTicks - previousTicks;
		previousTicks = newTicks;
		
		while (frameTime > 0)
		{
			float deltaTime = glm::min(frameTime, 1.0f);
			// Handle user input
			game.ProcessInput(deltaTime);
			// Update game state
			game.Update(deltaTime);

			frameTime -= deltaTime;
		}

		// Render
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		game.Render();

		window.swapBuffer();
	}

	return 0;
}
