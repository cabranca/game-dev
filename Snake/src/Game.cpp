#include <Game.h>
#include <CabrankEngine.h>
#include <ResourceManager.h>

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
			sUserInput();
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

	// Create Player
	auto snakePos = vec2(c_WindowWidth / 2 - c_SnakeSpriteSize.x / 2, c_WindowHeight / 2 - c_SnakeSpriteSize.y / 2);
	auto snakeTex = ResourceManager::getTexture(c_SnakeLabel);
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
	ResourceManager::loadTexture("textures/background.jpg", false, c_BackgroundLabel);
}

void Game::update(float delta)
{}

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
}

void Game::sCollision()
{

}

void Game::sRender()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	auto texture = ResourceManager::getTexture(c_BackgroundLabel);
	m_Renderer->drawSprite(texture, vec2(0.0f, 0.0f), vec2(c_WindowWidth, c_WindowHeight));
	
	m_Window.swapBuffer();
}
