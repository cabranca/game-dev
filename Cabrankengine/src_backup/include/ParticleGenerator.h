#pragma once

#include <Shader.h>
#include <Texture2D.h>
#include <GameObject.h>
#include <vector>

namespace cabrankengine {
	// Represents a single particle and its state
	struct Particle {
		glm::vec2 Position, Velocity;
		glm::vec4 Color;
		float Life;

		Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
	};

	// ParticleGenerator acts as a container for rendering a large number of 
	// particles by repeatedly spawning and updating particles and killing 
	// them after a given amount of time.
	class ParticleGenerator
	{
	public:
		ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount);

		void update(float delta, GameObject& object, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));

		void draw();

	private:
		std::vector<Particle> m_Particles;
		unsigned int m_LastUsedParticle;
		unsigned int m_Amount;
		Shader m_Shader;
		Texture2D m_Texture;
		unsigned int m_VAO;

		void init();
		unsigned int firstUnusedParticle();
		void respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
	};
}

