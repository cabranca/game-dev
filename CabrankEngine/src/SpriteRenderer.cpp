#include <SpriteRenderer.h>
#include <GL/glew.h>

using namespace cabrankengine;
using namespace glm;

SpriteRenderer::SpriteRenderer(Shader& shader) : m_Shader(shader), m_QuadVAO(0)
{
    initRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &m_QuadVAO);
}

void SpriteRenderer::drawSprite(Texture2D& texture, vec2 position, vec2 size, float rotate, vec3 color)
{
    // Prepare transformations
    m_Shader.use();
    auto model = mat4(1.0f);
    model = translate(model, vec3(position, 0.0f));

    model = translate(model, vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, radians(rotate), vec3(0.0f, 0.0f, 1.0f));
    model = translate(model, vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    model = scale(model, vec3(size, 1.0f));

    m_Shader.setMatrix4("model", model);
    m_Shader.setVector3f("spriteColor", color);

    glActiveTexture(GL_TEXTURE0);
    texture.bind();

    glBindVertexArray(m_QuadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void SpriteRenderer::initRenderData()
{
	// Configure VAO/VBO
	unsigned int VBO;
    float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &m_QuadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(m_QuadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
