#include <iostream>
#include <CabrankEngine.h>
#include <Window.h>

//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

constexpr unsigned int SCREEN_WIDTH = 800;
constexpr unsigned int SCREEN_HEIGHT = 600;

//breakout::Game breakoutGame(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char** argv) {
	cabrankengine::CabrankEngine::init();

	cabrankengine::Window window;
	window.create("Breakout", SCREEN_WIDTH, SCREEN_HEIGHT, 0);

	while (true)
	{
		window.swapBuffer();
	}

	return 0;
}
