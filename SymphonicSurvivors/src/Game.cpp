#include <Game.h>
#include <CabrankEngine.h>
#include <GL/glew.h>
#include <ResourceManager.h>

using namespace ss;
using namespace cabrankengine;

Game::Game() : m_CurrentState(ACTIVE), m_Window(), m_InputManager(), m_Renderer(), m_TextRenderer(), m_EntityManager() {}

void Game::run() {
	init();
}

void Game::init() {
	cabrankengine::CabrankEngine::init();

	m_Window.create("Snake", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

	loadShaders();
	loadTextures();

	auto shader = ResourceManager::getShader(SPRITE_LABEL);
	m_Renderer = std::make_unique<SpriteRenderer>(shader);

	// Create Text Renderer
	m_TextRenderer = std::make_unique<TextRenderer>(static_cast<unsigned int>(WINDOW_WIDTH), static_cast<unsigned int>(WINDOW_HEIGHT));
	m_TextRenderer->load("fonts/ocraext.TTF", 24);
}

void Game::loadShaders() {
}

void Game::loadTextures() {
}

void Game::update() {
}

void Game::sUserInput() {
}

void Game::sMovement() {
}

void Game::sCollision() {
}

void Game::sRender() {
	// This function would render the game state, but currently does nothing.
	// It can be implemented later to render the game entities and UI.
}


