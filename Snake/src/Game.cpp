#include <Game.h>
#include <CabrankEngine.h>
#include <ResourceManager.h>
#include <CollisionSolver.h>
#include <random>

using namespace glm;
using namespace cabrankengine;
using namespace snake;

Game::Game() : m_CurrentState(ACTIVE), m_Window(), m_InputManager(), m_Renderer(), m_EntityManager() {}

void Game::run()
{
	init();

	// deltaTime variables
	const float DESIRED_FRAME_TIME = 1.0f / 60.0f;
	float previousTicks = 0.0f;
	float frameTime = 0.0f;

	

	while (m_CurrentState != QUIT)
	{
		// Calculate delta time
		float newTicks = static_cast<float>(SDL_GetTicks()) / 1000.0f;
		frameTime = newTicks - previousTicks;
		previousTicks = newTicks;

		while (frameTime > 0)
		{
			float deltaTime = min(frameTime, 1.0f);
			update(deltaTime);

			frameTime -= deltaTime;
		}
		sRender();
	}
}

void Game::init()
{
	cabrankengine::CabrankEngine::init();

	m_Window.create("Snake", c_WindowWidth, c_WindowHeight, 0);
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

	loadShaders();
	loadTextures();

	auto shader = ResourceManager::getShader(c_SpriteLabel);
	m_Renderer = std::make_unique<SpriteRenderer>(shader);

	setBackground();
	spawnSnake();
	spawnFruit();
}

void Game::loadShaders()
{
	ResourceManager::loadShader("shaders/sprite.vs", "shaders/sprite.frag", nullptr, c_SpriteLabel);
	mat4 projection = ortho(0.0f, static_cast<float>(c_WindowWidth), static_cast<float>(c_WindowHeight), 0.0f, -1.0f, 1.0f);
	ResourceManager::getShader(c_SpriteLabel).use().setInteger("image", 0);
	ResourceManager::getShader(c_SpriteLabel).setMatrix4("projection", projection);
}

void Game::loadTextures()
{
	ResourceManager::loadTexture("textures/awesomeface.png", true, c_SnakeLabel);
	ResourceManager::loadTexture("textures/block.png", true, c_FruitLabel);
	ResourceManager::loadTexture("textures/background.jpg", false, c_BackgroundLabel);
}

void Game::update(float delta)
{
	m_EntityManager.update();
	sUserInput();
	sMovement(delta);
	sCollision();
	sRender();
}

void Game::sUserInput()
{
	m_InputManager.update();

	SDL_Event ev;
	while (SDL_PollEvent(&ev))
	{
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

	for (auto& e : m_EntityManager.getEntities(c_SnakeLabel))
	{
		/*
			This must only set the Input Component.
			If a direction is set, the other ones are disabled.
			If a direction is set, the opposite cannot be set next.
		*/

		auto newInput = CInput();
		auto& cInput = e->getComponent<CInput>();

		if (!cInput.right && m_InputManager.isKeyJustPressed(SDLK_a))
			newInput.left = true;
		else if (!cInput.left && m_InputManager.isKeyJustPressed(SDLK_d))
			newInput.right = true;
		else if (!cInput.down && m_InputManager.isKeyJustPressed(SDLK_w))
			newInput.up = true;
		else if (!cInput.up && m_InputManager.isKeyJustPressed(SDLK_s))
			newInput.down = true;
		else
			return;

		cInput = newInput;
	}
}

void Game::sMovement(float delta)
{
	for (auto& e : m_EntityManager.getEntities(c_SnakeLabel))
	{
		auto& cInput = e->getComponent<CInput>();
		if (cInput.left)
			e->getComponent<CTransform>().velocity = vec2(-1.0f, 0.0f);
		else if (cInput.right)
			e->getComponent<CTransform>().velocity = vec2(1.0f, 0.0f);
		else if (cInput.up)
			e->getComponent<CTransform>().velocity = vec2(0.0f, -1.0f);
		else if (cInput.down)
			e->getComponent<CTransform>().velocity = vec2(0.0f, 1.0f);

		e->getComponent<CTransform>().pos += e->getComponent<CTransform>().velocity * c_SnakeSpeed * delta;
	}
}

void Game::sCollision()
{
	for (auto& x : m_EntityManager.getEntities(c_SnakeLabel))
	{
		for (auto& y: m_EntityManager.getEntities(c_FruitLabel))
		{
			if (x->hasComponent<CTransform>() && x->hasComponent<CBoundingBox>() && y->hasComponent<CTransform>() && y->hasComponent<CBoundingBox>())
			{
				if (CollisionSolver::AABBCollision(x, y))
				{
					y->destroy();
					spawnFruit();
				}
			}
				
		}
	}
}

void Game::sRender()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (auto& entity : m_EntityManager.getEntities())
	{
		if (entity->hasComponent<CTexture>())
		{
			auto& cTexture = entity->getComponent<CTexture>();
			auto texture = ResourceManager::getTexture(cTexture.name);
			m_Renderer->drawSprite(texture, entity->getComponent<CTransform>().pos, cTexture.size, 0.0f, cTexture.color);
		}
	}
	
	m_Window.swapBuffer();
}

void Game::setBackground()
{
	auto background = m_EntityManager.addEntity(c_BackgroundLabel);
	background->addComponent<CTexture>(c_BackgroundLabel, vec2(), vec2(c_WindowWidth, c_WindowHeight), vec3(1.0));
}

void Game::spawnSnake()
{
	auto snake = m_EntityManager.addEntity(c_SnakeLabel);
	snake->addComponent<CTransform>(vec2(c_WindowWidth / 2, c_WindowHeight / 2), vec2(), 0);
	snake->addComponent<CBoundingBox>(c_SnakeSpriteSize);
	snake->addComponent<CTexture>(c_SnakeLabel, vec2(c_WindowWidth / 2, c_WindowHeight / 2), c_SnakeSpriteSize, vec3(1.0f));
}

void Game::spawnFruit()
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> randx(0, c_WindowWidth - c_FruitSpriteSize.x);
	std::uniform_int_distribution<std::mt19937::result_type> randy(0, c_WindowHeight - c_FruitSpriteSize.y);

	auto x = randx(rng);
	auto y = randy(rng);

	auto fruit = m_EntityManager.addEntity(c_FruitLabel);
	fruit->addComponent<CTransform>(vec2(x, y), vec2(), 0);
	fruit->addComponent<CBoundingBox>(c_FruitSpriteSize);
	fruit->addComponent<CTexture>(c_FruitLabel, vec2(x, y), c_FruitSpriteSize, vec3(1.0f));
}
