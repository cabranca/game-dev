#include <Game.h>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <SDL/SDL.h>
#include <CabrankEngine.h>
#include <ResourceManager.h>

using namespace ss;
using namespace cabrankengine;
using namespace glm;

Game::Game() : m_CurrentState(ACTIVE), m_Window(), m_InputManager(), m_Renderer(), m_TextRenderer(), m_EntityManager() {}

void Game::run() {
	init();

	constexpr float MAX_DELTA_TIME = 0.05f;
	constexpr float TARGET_FRAME_TIME = 1.0f / 60.0f; // ~0.01667s
	constexpr Uint32 TARGET_FRAME_MS = static_cast<Uint32>(TARGET_FRAME_TIME * 1000.0f);

	Uint32 previousTicks = SDL_GetTicks();

	while (m_CurrentState != QUIT)
	{
		Uint32 frameStart = SDL_GetTicks();

		float deltaTime = (frameStart - previousTicks) / 1000.0f;
		previousTicks = frameStart;

		if (deltaTime > MAX_DELTA_TIME)
			deltaTime = MAX_DELTA_TIME;

		sUserInput();
		update(deltaTime);
		sRender();

		Uint32 frameTime = SDL_GetTicks() - frameStart;
		if (frameTime < TARGET_FRAME_MS)
		{
			SDL_Delay(TARGET_FRAME_MS - frameTime);
		}
	}
}

void Game::init() {
	cabrankengine::CabrankEngine::init();

	m_Window.create("Symphonic Survivors", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
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
	ResourceManager::loadShader("shaders/sprite.vs", "shaders/sprite.frag", "", SPRITE_LABEL);
	mat4 projection = ortho(0.0f, static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT), 0.0f, -1.0f, 1.0f);
	ResourceManager::getShader(SPRITE_LABEL).use().setInteger("image", 0);
	ResourceManager::getShader(SPRITE_LABEL).setMatrix4("projection", projection);
}

void Game::loadTextures() {
}

void Game::update(float deltaTime) {
	m_EntityManager.update();
	sMovement();
	sCollision();
}

void Game::sUserInput() {
	m_InputManager.update();

	SDL_Event ev;
	while (SDL_PollEvent(&ev)) {
		switch (ev.type)
		{
		case SDL_QUIT:
			m_CurrentState = QUIT;
			break;
		case SDL_KEYDOWN:
			m_InputManager.pressKey(ev.key.keysym.sym);
			break;
		case SDL_KEYUP:
			m_InputManager.releaseKey(ev.key.keysym.sym);
			break;
		default:
			break;
		}
	}

	if (m_InputManager.isKeyJustPressed(SDLK_ESCAPE))
		m_CurrentState = QUIT;
}

void Game::sMovement() {
}

void Game::sCollision() {
}

void Game::sRender() {
	glClear(GL_COLOR_BUFFER_BIT);

	if (m_CurrentState == ACTIVE)
	{
		for (auto& entity : m_EntityManager.getEntities())
		{
			if (entity->isActive() && entity->hasComponent<CTexture>())
			{
				auto& cTexture = entity->getComponent<CTexture>();
				auto texture = ResourceManager::getTexture(cTexture.name);
				m_Renderer->drawSprite(texture, entity->getComponent<CTransform>().pos, cTexture.size, 0.0f, cTexture.color);
			}
		}
	}
	else if (m_CurrentState == LOST)
	{
		m_TextRenderer->renderText(
			"You LOST :(", 320.0f, static_cast<float>(WINDOW_HEIGHT / 2) - 20.0f, 1.0f, vec3(0.0f, 1.0f, 0.0f)
		);
		m_TextRenderer->renderText(
			"Press ENTER to retry or ESC to quit", 130.0f, static_cast<float>(WINDOW_HEIGHT / 2), 1.0f, vec3(1.0f, 1.0f, 0.0f)
		);
	}

	m_Window.swapBuffer();
}


