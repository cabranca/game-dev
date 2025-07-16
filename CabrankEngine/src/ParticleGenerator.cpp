#include <ParticleGenerator.h>
#include <GL/glew.h>

using namespace cabrankengine;
using namespace glm;

ParticleGenerator::ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount)
	: m_Shader(shader), m_Texture(texture), m_Amount(amount), m_LastUsedParticle(0)
{
	init();
}

void ParticleGenerator::update(float delta, GameObject& object, unsigned int newParticles, vec2 offset)
{
	for (unsigned int i = 0; i < newParticles; i++) {
		int unusedParticle = firstUnusedParticle();
		respawnParticle(m_Particles[unusedParticle], object, offset);
	}

	for (unsigned int i = 0; i < m_Amount; i++) {
		Particle& p = m_Particles[i];
		p.Life -= delta;
		if (p.Life > 0.0f) {
			p.Position -= p.Velocity * delta;
			p.Color.a -= delta * 2.5f;
		}
	}
}

void ParticleGenerator::draw()
{
	// use additive blending to give it a 'glow' effect
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	m_Shader.use();
	for (auto& particle : m_Particles) {
		if (particle.Life > 0.0) {
			m_Shader.setVector2f("offset", particle.Position);
			m_Shader.setVector4f("color", particle.Color);
			m_Texture.bind();
			glBindVertexArray(m_VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}

	// don't forget to reset to default blending mode
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init()
{
	// set up mesh and attribute properties
	unsigned int VBO;
	float particle_quad[] = {
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(m_VAO);

	// Fill mesh buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);

	// Set mesh attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindVertexArray(0);

	// Create m_Amount default particle instances
	for (unsigned int i = 0; i < m_Amount; i++)
		m_Particles.emplace_back();
}

unsigned int ParticleGenerator::firstUnusedParticle()
{
	// Search from last used particle, this will usually return almost instantly
	for (unsigned int i = m_LastUsedParticle; i < m_Amount; i++) {
		if (m_Particles[i].Life <= 0.0f) {
			m_LastUsedParticle = i;
			return i;
		}
	}

	// Otherwise, do a linear search from the start
	for (unsigned int i = 0; i < m_LastUsedParticle; i++) {
		if (m_Particles[i].Life <= 0.0f) {
			m_LastUsedParticle = i;
			return i;
		}
	}

	// All particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
	m_LastUsedParticle = 0;
	return 0;
}

void ParticleGenerator::respawnParticle(Particle& particle, GameObject& object, vec2 offset)
{
	float random = ((rand() % 100) - 50) / 10.0f;
	float rColor = 0.5f + ((rand() % 100) / 100.0f);
	particle.Position = object.GetPosition() + random + offset;
	particle.Color = vec4(rColor, rColor, rColor, 1.0f);
	particle.Life = 1.0f;
	particle.Velocity = object.GetVelocity() * 0.1f;
}
