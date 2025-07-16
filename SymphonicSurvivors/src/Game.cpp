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

	spawnPlayer();
}

void Game::loadShaders() {
	ResourceManager::loadShader("shaders/sprite.vs", "shaders/sprite.frag", "", SPRITE_LABEL);
	mat4 projection = ortho(0.0f, static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT), 0.0f, -1.0f, 1.0f);
	ResourceManager::getShader(SPRITE_LABEL).use().setInteger("image", 0);
	ResourceManager::getShader(SPRITE_LABEL).setMatrix4("projection", projection);
}

void Game::loadTextures() {
	ResourceManager::loadTexture("textures/awesomeface.png", true, PLAYER_LABEL);
}

void Game::update(float deltaTime) {
	m_EntityManager.update();
	sMovement(deltaTime);
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

	auto input = CInput();
	if (m_InputManager.isKeyDown(SDLK_a))
		input.left = true;
	else if (m_InputManager.isKeyDown(SDLK_d))
		input.right = true;
	if (m_InputManager.isKeyDown(SDLK_w))
		input.up = true;
	else if (m_InputManager.isKeyDown(SDLK_s))
		input.down = true;
	auto& inputComponent = m_Player->getComponent<CInput>();
	inputComponent = input;
}

void Game::sMovement(float deltaTime) {
	// Handle player movement.
	auto playerDirection = glm::vec2();
	const auto& input = m_Player->getComponent<CInput>();
	if (input.up)
		playerDirection.y = -1;
	else if (input.down)
		playerDirection.y = 1;
	if (input.left)
		playerDirection.x = -1;
	else if (input.right)
		playerDirection.x = 1;
	glm::normalize(playerDirection);

	auto& transform = m_Player->getComponent<CTransform>();
	transform.pos += playerDirection * PLAYER_SPEED * deltaTime;

	// Handle bullets and enemies movement.
	for (auto& entity : m_EntityManager.getEntities()) {
		if (!entity->hasComponent<CPlayer>() && entity->hasComponent<CLinearMovement>()) {
			auto& transform = entity->getComponent<CTransform>();
			const auto& movement = entity->getComponent<CLinearMovement>();
			transform.pos += movement.direction * movement.speed * deltaTime;
		}
	}
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

void ss::Game::spawnPlayer() {
	const auto& player = m_EntityManager.addEntity(PLAYER_LABEL);
	player->addComponent<CPlayer>();
	player->addComponent<CTransform>(vec2(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f), vec2(), 0.f);
	player->addComponent<CBoundingBox>(PLAYER_SPRITE_SIZE);
	player->addComponent<CTexture>(PLAYER_LABEL, vec2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), PLAYER_SPRITE_SIZE, vec3(1.0f));
	player->addComponent<CInput>();
	m_Player = player;
}


