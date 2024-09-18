#pragma once
#include <Shader.h>
#include <Texture2D.h>

namespace cabrankengine {
	class PostProcessor
	{
	public:
		PostProcessor(Shader shader, unsigned int width, unsigned int height);

		// Prepares the postprocessor's framebuffer operations before rendering the game
		void beginRender();
		// Should be called after rendering the game, so it stores all the rendered data into a texture object
		void endRender();
		// Renders the PostProcessor texture quad (as a screen-encompassing large sprite)
		void render(float time);

		bool isChaos() const { return m_Chaos; }
		bool isConfuse() const { return m_Confuse; }
		void setConfuse(bool value) { m_Confuse = value; }
		void setChaos(bool value) { m_Chaos = value; }
		void setShake(bool value) { m_Shake = value; }

	private:
		unsigned int m_Msfbo, m_Fbo;// MSFBO = Multisampled FBO. FBO is regular, used for blitting MS color-buffer to texture
		unsigned int m_Rbo; // RBO is used for multisampled color buffer
		unsigned int m_Vao;

		Shader m_PostProcessingShader;
		Texture2D m_Texture;
		unsigned int m_Width, m_Height;
		bool m_Confuse, m_Chaos, m_Shake;

		// Initialize quad for rendering postprocessing texture
		void initRenderData();
	};
}

