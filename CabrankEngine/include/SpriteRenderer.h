#pragma once
#include <Shader.h>
#include <Texture2D.h>

namespace cabrankengine {
	class SpriteRenderer
	{
	public:
		SpriteRenderer() = default;
		SpriteRenderer(Shader& shader);
		~SpriteRenderer();

		void drawSprite(Texture2D& texture, const glm::vec2& position, const glm::vec2& size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, const glm::vec3& color = glm::vec3(1.0f));

	private:
		Shader m_Shader;
		unsigned int m_QuadVAO;

		void initRenderData();
	};
}


