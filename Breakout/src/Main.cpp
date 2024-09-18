#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Game.h>
#include <ResourceManager.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

constexpr unsigned int SCREEN_WIDTH = 800;
constexpr unsigned int SCREEN_HEIGHT = 600;

breakout::Game breakoutGame(SCREEN_WIDTH, SCREEN_HEIGHT);

int main() {

	// Initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, false);

	// Create the window object
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout", nullptr, nullptr);

	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Set the window context as current context for the thread
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADLoadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Set callbacks
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);

	// Set OpenGL configuration
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Initialize the game
	breakoutGame.Init();

	// deltaTime variables
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	// Window loop
	while (!glfwWindowShouldClose(window)) {
		// Calculate delta time
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		// Handle user input
		breakoutGame.ProcessInput(deltaTime);

		// Update game state
		breakoutGame.Update(deltaTime);

		// Render
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		breakoutGame.Render();

		glfwSwapBuffers(window);
	}

	// Delete all resources as Loaded using the resource manager
	ResourceManager::clear();

	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	// The escape key closes the app
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			breakoutGame.SetKey(key, true);
		else if (action == GLFW_RELEASE)
		{
			breakoutGame.SetKey(key, false);
			breakoutGame.ReleaseKey(key);
		}
	}
}
