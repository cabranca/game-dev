#include <Game.h>
#include <CabrankEngine.h>
#include <ResourceManager.h>
#include <CollisionSolver.h>
#include <random>
#include <iostream>

using namespace glm;
using namespace cabrankengine;
using namespace snake;

Game::Game() : m_CurrentState(ACTIVE), m_Window(), m_InputManager(), m_Renderer(), m_EntityManager(), m_LastInput(0) {}

void Game::run()
{
	init();

	// deltaTime variables
	const float DESIRED_FRAME_TIME = 1.0f / 4.0f;
	float previousTicks = 0.0f;
	float frameTime = 0.0f;
	float deltaTime = 0.0f;
	float newTicks = 0.0f;
	

	while (m_CurrentState != QUIT)
	{
		newTicks = static_cast<float>(SDL_GetTicks()) / 1000.0f;
		frameTime = newTicks - previousTicks;
		while (frameTime < DESIRED_FRAME_TIME)
		{
			// Calculate delta time
			newTicks = static_cast<float>(SDL_GetTicks()) / 1000.0f;
			frameTime = newTicks - previousTicks;
			
			sUserInput();
		}
		previousTicks = newTicks;
		update();
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

void Game::update()
{
	m_EntityManager.update();

	auto snake = m_EntityManager.getEntities(c_SnakeLabel);

	for (int i = 0; i < snake.size(); i++)
	{
		m_RegisteredInputs[i].push(m_LastInput);
		auto newInput = CInput();
		switch (m_RegisteredInputs[i].front())
		{
		case SDLK_a:
			newInput.left = true;
			break;
		case SDLK_d:
			newInput.right = true;
			break;
		case SDLK_w:
			newInput.up = true;
			break;
		case SDLK_s:
			newInput.down = true;
			break;
		}

		snake[i]->getComponent<CInput>() = newInput;
	}

	sMovement();
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

	auto& cInput = m_Head->getComponent<CInput>();

	if (!cInput.right && m_InputManager.isKeyJustPressed(SDLK_a))
		m_LastInput = SDLK_a;
	else if (!cInput.left && m_InputManager.isKeyJustPressed(SDLK_d))
		m_LastInput = SDLK_d;
	else if (!cInput.down && m_InputManager.isKeyJustPressed(SDLK_w))
		m_LastInput = SDLK_w;
	else if (!cInput.up && m_InputManager.isKeyJustPressed(SDLK_s))
		m_LastInput = SDLK_s;
}

void Game::sMovement()
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

		auto& pos = e->getComponent<CTransform>().pos;
		pos += e->getComponent<CTransform>().velocity * c_SnakeSpeed;

		if (pos.x >= c_WindowWidth)
			pos.x = 0.0f;
		else if (pos.x < 0.0f)
			pos.x = c_WindowWidth - c_SnakeSpriteSize.x;
		if (pos.y >= c_WindowHeight)
			pos.y = 0.0f;
		else if (pos.y < 0.0f)
			pos.y = c_WindowHeight - c_SnakeSpriteSize.y;
	}
}

void Game::sCollision()
{
	for (auto& e : m_EntityManager.getEntities(c_SnakeLabel))
	{
		if (e->getId() != m_Head->getId() && CollisionSolver::AABBCollision(e, m_Head))
		{
			//std::cout << "You Lost!" << std::endl;
		}
	}

	int leaveTail = 0;

	for (auto& e : m_EntityManager.getEntities(c_FruitLabel))
	{
		if (CollisionSolver::AABBCollision(e, m_Head))
		{
			e->destroy();
			spawnFruit();
			increaseTail();
			leaveTail++;
		}
	}

	

	for (int i = 0; i < m_RegisteredInputs.size() - leaveTail; i++)
		m_RegisteredInputs[i].pop();
}

void Game::sRender()
{
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
	m_Head = m_Tail = snake;
	m_RegisteredInputs.emplace_back();
}

void Game::spawnFruit()
{
	// TODO: prevent spawning in snake position
	// IDEA: random row, random column and check no overlap
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

void Game::increaseTail()
{
	auto snake = m_EntityManager.addEntity(c_SnakeLabel);
	auto newPos = m_Tail->getComponent<CTransform>().pos - m_Tail->getComponent<CTransform>().velocity * c_SnakeSpriteSize.x;
	snake->addComponent<CTransform>(newPos, vec2(), 0);
	snake->addComponent<CBoundingBox>(c_SnakeSpriteSize);
	snake->addComponent<CTexture>(c_SnakeLabel, newPos, c_SnakeSpriteSize, vec3(1.0f));

	m_RegisteredInputs.push_back(m_RegisteredInputs[m_RegisteredInputs.size() - 1]);
	//m_RegisteredInputs[m_RegisteredInputs.size() - 1].push(m_LastInput);
	m_Tail = snake;
}
