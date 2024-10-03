#include <Game.h>
#include <ResourceManager.h>
#include <GL/glew.h>
#include <ParticleGenerator.h>
#include <sstream>
#include <iostream>
#include <SDL/SDL_timer.h>
#include <SDL/SDL_events.h>

using namespace breakout;
using namespace glm;

Game::Game(unsigned int width, unsigned int height) 
	: m_Width(width), m_Height(height), State(GAME_MENU), m_Keys(), m_KeysProcessed(), m_CurrentLevel(0), m_Lives(3)
{
	m_InputManager = std::make_unique<InputManager>();
}

Game::~Game() {}

void Game::Init()
{
	// Start audio engine and play the music
	m_SoundEngine = std::unique_ptr<irrklang::ISoundEngine>(irrklang::createIrrKlangDevice());
	m_SoundEngine->play2D("audio/breakout.mp3", true);

	ResourceManager::loadShader("shaders/sprite.vs", "shaders/sprite.frag", nullptr, "sprite");
	ResourceManager::loadShader("shaders/particle.vs", "shaders/particle.frag", nullptr, "particle");
	ResourceManager::loadShader("shaders/post_processing.vs", "shaders/post_processing.frag", nullptr, "postprocessing");

	
	// Configure shaders
	mat4 projection = ortho(0.0f, static_cast<float>(m_Width), static_cast<float>(m_Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::getShader("sprite").use().setInteger("image", 0);
	ResourceManager::getShader("sprite").setMatrix4("projection", projection);
	ResourceManager::getShader("particle").use().setMatrix4("projection", projection);

	// Load Textures
	ResourceManager::loadTexture("textures/background.jpg", false, "background");
	ResourceManager::loadTexture("textures/awesomeface.png", true, "face");
	ResourceManager::loadTexture("textures/block.png", true, "block");
	ResourceManager::loadTexture("textures/block_solid.png", false, "block_solid");
	ResourceManager::loadTexture("textures/paddle.png", false, "paddle");
	ResourceManager::loadTexture("textures/particle.png", true, "particle");
	ResourceManager::loadTexture("textures/powerup_speed.png", true, "powerup_speed");
	ResourceManager::loadTexture("textures/powerup_sticky.png", true, "powerup_sticky");
	ResourceManager::loadTexture("textures/powerup_increase.png", true, "powerup_increase");
	ResourceManager::loadTexture("textures/powerup_confuse.png", true, "powerup_confuse");
	ResourceManager::loadTexture("textures/powerup_chaos.png", true, "powerup_chaos");
	ResourceManager::loadTexture("textures/powerup_passthrough.png", true, "powerup_passthrough");

	// Set render-specific controls
	auto shader = ResourceManager::getShader("sprite");
	m_Renderer = std::make_unique<SpriteRenderer>(shader);

	auto partShader = ResourceManager::getShader("particle");
	auto partTex = ResourceManager::getTexture("particle");
	m_Particles = std::make_unique<ParticleGenerator>(partShader, partTex, 500);

	auto effectshader = ResourceManager::getShader("postprocessing");
	m_Effects = std::make_unique<PostProcessor>(effectshader, m_Width, m_Height);

	// Load levels
	GameLevel one, two, three, four; 
	one.Load("levels/one.lvl", m_Width, m_Height / 2);
	two.Load("levels/two.lvl", m_Width, m_Height / 2);
	three.Load("levels/three.lvl", m_Width, m_Height / 2);
	four.Load("levels/four.lvl", m_Width, m_Height / 2);
	m_Levels.push_back(one);
	m_Levels.push_back(two);
	m_Levels.push_back(three);
	m_Levels.push_back(four);
	m_CurrentLevel = 2;

	// Create Player
	auto playerPos = vec2(m_Width / 2.0f - c_PlayerSize.x / 2.0f, m_Height - c_PlayerSize.y);
	auto playerTex = ResourceManager::getTexture("paddle");
	m_Player = std::make_unique<Player>(playerPos, c_PlayerSize, playerTex);

	// Create Ball
	auto ballPos = playerPos + vec2(c_PlayerSize.x / 2.0f - c_BallRadius, -c_BallRadius * 2.0f);
	auto ballTex = ResourceManager::getTexture("face");
	m_Ball = std::make_unique<Ball>(ballPos, c_BallRadius, c_InitialBallVelocity, ballTex);	

	// Create Text Renderer
	m_TextRenderer = std::make_unique<TextRenderer>(m_Width, m_Height);
	m_TextRenderer->load("fonts/ocraext.TTF", 24);
}

void Game::ProcessInput(float delta)
{
	m_InputManager->update();

	SDL_Event evnt;
	//Will keep looping until there are no more events to process
	while (SDL_PollEvent(&evnt))
	{
		switch (evnt.type)
		{
		case SDL_QUIT:
			State = GAME_QUIT;
			break;
		case SDL_KEYDOWN:
			m_InputManager->pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			m_InputManager->releaseKey(evnt.key.keysym.sym);
			break;
		}
	}

	if (State == GAME_ACTIVE) {
		float velocity = c_PlayerVelocity * delta;
		
		if (m_InputManager->isKeyDown(SDLK_a) && m_Player->GetPosition().x >= 0.0f) {
			m_Player->GetPosition().x -= velocity;
			if (m_Ball->isStuck())
				m_Ball->GetPosition().x -= velocity;
		}
			
		if (m_InputManager->isKeyDown(SDLK_d) && m_Player->GetPosition().x <= m_Width - m_Player->GetSize().x) {
			m_Player->GetPosition().x += velocity;
			if (m_Ball->isStuck())
				m_Ball->GetPosition().x += velocity;
		}

		if (m_InputManager->isKeyDown(SDLK_SPACE))
			m_Ball->setStuck(false);
		if (m_InputManager->isKeyJustPressed(SDLK_ESCAPE))
			State = GAME_QUIT;
	}
	if (State == GAME_MENU)
	{
		if (m_InputManager->isKeyDown(SDLK_RETURN))
			State = GAME_ACTIVE;
		if (m_InputManager->isKeyJustPressed(SDLK_w))
			m_CurrentLevel = (m_CurrentLevel + 1) % 4;
		if (m_InputManager->isKeyJustPressed(SDLK_s)) {
			if (m_CurrentLevel > 0)
				--m_CurrentLevel;
			else
				m_CurrentLevel = 3;
		}
	}
	if (State == GAME_WIN)
	{
		if (m_InputManager->isKeyJustPressed(SDLK_RETURN))
		{
			m_Effects->setChaos(false);
			State = GAME_MENU;
		}
	}
}

void Game::Update(float delta)
{
	if (State == GAME_ACTIVE && m_Levels[m_CurrentLevel].IsCompleted())
	{
		ResetLevel();
		ResetPlayer();
		m_Effects->setChaos(true);
		State = GAME_WIN;
	}

	// Update objects
	m_Ball->Move(delta, m_Width);

	// Check for collisions
	DoCollisions();

	// Update particles
	m_Particles->update(delta, *m_Ball, 2, vec2(m_Ball->getRadius() / 2.0f));

	// update PowerUps
	UpdatePowerUps(delta);

	// reduce shake time
	if (m_ShakeTime > 0.0f)
	{
		m_ShakeTime -= delta;
		if (m_ShakeTime <= 0.0f)
			m_Effects->setShake(false);
	}

	// check loss condition
	if (m_Ball->GetPosition().y >= m_Height) // did ball reach bottom edge?
	{
		--m_Lives;
		// did the player lose all his lives? : Game over
		if (m_Lives == 0)
		{
			ResetLevel();
			State = GAME_MENU;
		}
		ResetPlayer();
	}
}

void Game::Render()
{
	if (State == GAME_WIN)
	{
		m_TextRenderer->renderText(
			"You WON!!!", 320.0f, static_cast<float>(m_Height / 2) - 20.0f, 1.0f, vec3(0.0f, 1.0f, 0.0f)
		);
		m_TextRenderer->renderText(
			"Press ENTER to retry or ESC to quit", 130.0f, static_cast<float>(m_Height / 2), 1.0f, vec3(1.0f, 1.0f, 0.0f)
		);
	}
	if (State == GAME_ACTIVE || State == GAME_MENU) {
		m_Effects->beginRender();

		auto texture = ResourceManager::getTexture("background");
		m_Renderer->drawSprite(texture, vec2(0.0f, 0.0f), vec2(m_Width, m_Height), 0.0f);

		m_Levels[m_CurrentLevel].Draw(*m_Renderer);
		m_Player->draw(*m_Renderer);
		// draw PowerUps
		for (auto& powerUp : m_PowerUps)
			if (!powerUp->IsDestroyed())
				powerUp->draw(*m_Renderer);
		m_Particles->draw();
		m_Ball->draw(*m_Renderer);

		m_Effects->endRender();

		m_Effects->render(static_cast<float>(SDL_GetTicks()) / 1000.0f);

		std::stringstream ss;
		ss << m_Lives;
		m_TextRenderer->renderText("Lives:" + ss.str(), 5.0f, 5.0f, 1.0f);
	}
	if (State == GAME_MENU)
	{
		m_TextRenderer->renderText("Press ENTER to start", 250.0f, static_cast<float>(m_Height / 2), 1.0f);
		m_TextRenderer->renderText("Press W or S to select level", 245.0f, static_cast<float>(m_Height / 2) + 20.0f, 0.75f);
	}
}

void Game::ResetLevel()
{
	m_Lives = 3;
	if (m_CurrentLevel == 0)
		m_Levels[0].Load("levels/one.lvl", m_Width, m_Height / 2);
	else if (m_CurrentLevel == 1)
		m_Levels[1].Load("levels/two.lvl", m_Width, m_Height / 2);
	else if (m_CurrentLevel == 2)
		m_Levels[2].Load("levels/three.lvl", m_Width, m_Height / 2);
	else if (m_CurrentLevel == 3)
		m_Levels[3].Load("levels/four.lvl", m_Width, m_Height / 2);
}

void Game::ResetPlayer()
{
	// reset player/ball stats
	m_Player->reset(vec2(m_Width / 2.0f - c_PlayerSize.x / 2.0f, m_Height - c_PlayerSize.y), c_PlayerSize, vec2());
	m_Ball->reset(m_Player->GetPosition() + vec2(c_PlayerSize.x / 2.0f - c_BallRadius, -(c_BallRadius * 2.0f)), m_Ball->GetSize(), c_InitialBallVelocity);

	// also disable all active powerups
	m_Effects->setChaos(false);
	m_Effects->setConfuse(false);
	m_Ball->setPassThrough(false);
	m_Ball->setSticky(false);
	m_Player->SetColor(vec3(1.0f));
	m_Ball->SetColor(vec3(1.0f));
}

void Game::UpdatePowerUps(float delta)
{
	for (auto& powerUp : m_PowerUps)
	{
		powerUp->GetPosition() += powerUp->GetVelocity() * delta;
		if (powerUp->IsActivated())
		{
		powerUp->DecreaseDuration(delta);

			if (powerUp->GetDuration() <= 0.0f)
			{
				// remove powerup from list (will later be removed)
				powerUp->SetActivated(false);
				// deactivate effects
				if (powerUp->GetType() == "sticky")
				{
					if (!IsOtherPowerUpActive(m_PowerUps, "sticky"))
					{	// only reset if no other PowerUp of type sticky is active
						m_Ball->setSticky(false);
						m_Player->SetColor(vec3(1.0f));
					}
				}
				else if (powerUp->GetType() == "pass-through")
				{
					if (!IsOtherPowerUpActive(m_PowerUps, "pass-through"))
					{	// only reset if no other PowerUp of type pass-through is active
						m_Ball->setPassThrough(false);
						m_Ball->SetColor(vec3(1.0f));
					}
				}
				else if (powerUp->GetType() == "confuse")
				{
					if (!IsOtherPowerUpActive(m_PowerUps, "confuse"))
					{	// only reset if no other PowerUp of type confuse is active
						m_Effects->setConfuse(false);
					}
				}
				else if (powerUp->GetType() == "chaos")
				{
					if (!IsOtherPowerUpActive(m_PowerUps, "chaos"))
					{	// only reset if no other PowerUp of type chaos is active
						m_Effects->setChaos(false);
					}
				}
			}
		}
	}

	// Remove all PowerUps from vector that are destroyed AND !activated (thus either off the map or finished)
	// Note we use a lambda expression to remove each PowerUp which is destroyed and not activated
	std::erase_if(m_PowerUps, [](std::shared_ptr<PowerUp> powerUp) { return powerUp->IsDestroyed() && !powerUp->IsActivated(); });
}

bool Game::ShouldSpawn(unsigned int chance)
{
	unsigned int random = rand() % chance;
	return random == 0;
}
void Game::SpawnPowerUps(GameObject& block)
{
	if (ShouldSpawn(75)) // 1 in 75 chance
		m_PowerUps.emplace_back(std::make_shared<PowerUp>("speed", vec3(0.5f, 0.5f, 1.0f), 0.0f, block.GetPosition(), ResourceManager::getTexture("powerup_speed")));
	if (ShouldSpawn(75))
		m_PowerUps.emplace_back(std::make_shared<PowerUp>("sticky", vec3(1.0f, 0.5f, 1.0f), 20.0f, block.GetPosition(), ResourceManager::getTexture("powerup_sticky")));
	if (ShouldSpawn(75))
		m_PowerUps.emplace_back(std::make_shared<PowerUp>("pass-through", vec3(0.5f, 1.0f, 0.5f), 10.0f, block.GetPosition(), ResourceManager::getTexture("powerup_passthrough")));
	if (ShouldSpawn(75))
		m_PowerUps.emplace_back(std::make_shared<PowerUp>("pad-size-increase", vec3(1.0f, 0.6f, 0.4), 0.0f, block.GetPosition(), ResourceManager::getTexture("powerup_increase")));
	if (ShouldSpawn(15)) // Negative powerups should spawn more often
		m_PowerUps.emplace_back(std::make_shared<PowerUp>("confuse", vec3(1.0f, 0.3f, 0.3f), 15.0f, block.GetPosition(), ResourceManager::getTexture("powerup_confuse")));
	if (ShouldSpawn(15))
		m_PowerUps.emplace_back(std::make_shared<PowerUp>("chaos", vec3(0.9f, 0.25f, 0.25f), 15.0f, block.GetPosition(), ResourceManager::getTexture("powerup_chaos")));
}

void Game::ActivatePowerUp(PowerUp& powerUp)
{
	if (powerUp.GetType() == "speed")
	{
		m_Ball->accelerate(1.2f);
	}
	else if (powerUp.GetType() == "sticky")
	{
		m_Ball->setSticky(true);
		m_Player->SetColor(vec3(1.0f, 0.5f, 1.0f));
	}
	else if (powerUp.GetType() == "pass-through")
	{
		m_Ball->setPassThrough(true);
		m_Ball->SetColor(vec3(1.0f, 0.5f, 0.5f));
	}
	else if (powerUp.GetType() == "pad-size-increase")
	{
		m_Player->Extend(50);
	}
	else if (powerUp.GetType() == "confuse")
	{
		if (!m_Effects->isChaos())
			m_Effects->setConfuse(true); // only activate if chaos wasn't already active
	}
	else if (powerUp.GetType() == "chaos")
	{
		if (!m_Effects->isConfuse())
			m_Effects->setChaos(true);
	}
}

bool Game::IsOtherPowerUpActive(std::vector<std::shared_ptr<PowerUp>>& powerUps, std::string type)
{
	// Check if another PowerUp of the same type is still active
	// in which case we don't disable its effect (yet)
	for (const auto& powerUp : powerUps)
	{
		if (powerUp->IsActivated())
			if (powerUp->GetType() == type)
				return true;
	}
	return false;
}

void Game::DoCollisions()
{
	// Check for collisions between ball and bricks
	for (auto& brick : m_Levels[m_CurrentLevel].GetBricks()) {
		if (!brick.IsDestroyed()) {
			auto [collisionDetected, collisionSide, diffVector] = checkCollision(*m_Ball, brick);
			if (collisionDetected) {
				if (!brick.IsSolid()) {
					brick.Destroy();
					SpawnPowerUps(brick);
					m_SoundEngine->play2D("audio/bleep.mp3", false);
				}
			
				else {
					// If block is solid, enable shake effect
					m_ShakeTime = 0.05f;
					m_Effects->setShake(true);
					m_SoundEngine->play2D("audio/solid.wav", false);
				}

				// Collision resolution
				if (collisionSide == LEFT || collisionSide == RIGHT) {
					m_Ball->bounceToWall(Axis::H);
					
					// Relocate
					float penetration = m_Ball->getRadius() - std::abs(diffVector.x);
					if (collisionSide == LEFT)
						m_Ball->GetPosition().x += penetration;
					else
						m_Ball->GetPosition().x -= penetration;
				}
				else {
					m_Ball->bounceToWall(Axis::V);

					// Relocate
					float penetration = m_Ball->getRadius() - std::abs(diffVector.y);
					if (collisionSide == UP)
						m_Ball->GetPosition().y -= penetration;
					else
						m_Ball->GetPosition().y += penetration;
				}
			}
		}
	}

	// also check collisions on PowerUps and if so, activate them
	for (auto& powerUp : m_PowerUps)
	{
		if (!powerUp->IsDestroyed())
		{
			// first check if powerup passed bottom edge, if so: keep as inactive and destroy
			if (powerUp->GetPosition().y >= m_Height)
				powerUp->Destroy();

			if (checkCollision(*m_Player, *powerUp))
			{	// collided with player, now activate powerup
				ActivatePowerUp(*powerUp);
				powerUp->Destroy();
				powerUp->SetActivated(true);
				m_SoundEngine->play2D("audio/powerup.wav", false);
			}
		}
	}

	// Check for collisions between ball and player
	auto [collisionDetected, collisionSide, diffVector] = checkCollision(*m_Ball, *m_Player);
	if (!m_Ball->isStuck() && collisionDetected) {
		float centerBoard = m_Player->GetPosition().x + m_Player->GetSize().x / 2.0f;
		float distance = (m_Ball->GetPosition().x + m_Ball->getRadius()) - centerBoard;
		float percentage = distance / (m_Player->GetSize().x / 2.0f);

		m_Ball->bounceToPlayer(percentage);
		
		m_Ball->setStuck(m_Ball->isSticky());

		m_SoundEngine->play2D("audio/bleep.wav", false);
	}
}

bool Game::checkCollision(GameObject& one, GameObject& two)
{
	// collision x-axis?
	bool collisionX = one.GetPosition().x + one.GetSize().x >= two.GetPosition().x &&
		two.GetPosition().x + two.GetSize().x >= one.GetPosition().x;
	// collision y-axis?
	bool collisionY = one.GetPosition().y + one.GetSize().y >= two.GetPosition().y &&
		two.GetPosition().y + two.GetSize().y >= one.GetPosition().y;
	// collision only if on both axes
	return collisionX && collisionY;
}

Collision Game::checkCollision(Ball& one, GameObject& two) {
	// get center point circle first 
	vec2 center(one.GetPosition() + one.getRadius());
	// calculate AABB info (center, half-extents)
	vec2 aabb_half_extents(two.GetSize().x / 2.0f, two.GetSize().y / 2.0f);
	vec2 aabb_center(
		two.GetPosition().x + aabb_half_extents.x,
		two.GetPosition().y + aabb_half_extents.y
	);
	// get difference vector between both centers
	vec2 difference = center - aabb_center;
	vec2 clamped = clamp(difference, -aabb_half_extents, aabb_half_extents);
	// add clamped value to AABB_center and we get the value of box closest to circle
	vec2 closest = aabb_center + clamped;
	// retrieve vector between center circle and closest point AABB and check if length <= radius
	difference = closest - center;
	if (length(difference) < one.getRadius())
		return std::make_tuple(true, vectorDirection(difference), difference);
	else
		return std::make_tuple(false, UP, vec2(0.0f, 0.0f));
}

Direction Game::vectorDirection(vec2 target)
{
	vec2 compass[] = {
		vec2(0.0f, 1.0f),	// up
		vec2(1.0f, 0.0f),	// right
		vec2(0.0f, -1.0f),	// down
		vec2(-1.0f, 0.0f)	// left
	};

	float max = 0.0f;
	unsigned int bestMatch = -1;
	for (unsigned int i = 0; i < 4; i++) {
		float dotProduct = dot(normalize(target), compass[i]);
		if (dotProduct > max) {
			max = dotProduct;
			bestMatch = i;
		}
	}
	return static_cast<Direction>(bestMatch);
}