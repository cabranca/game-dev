#include "Outliner.h"

#include <glad/glad.h>

#include <Cabrankengine/Math/MatrixFactory.h>

#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"

namespace cabrankengine::rendering {

    using namespace math;

	void Outliner::init() {
		m_Shader = Shader::create("assets/shaders/Outline.glsl");
	}

	void Outliner::drawOutline(const Ref<Shader>& originalShader, const Ref<VertexArray>& vertexArray, const Color& outlineColor, const math::Mat4& transform) {		
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        Renderer::submit(originalShader, vertexArray, transform);
        
        
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);

        m_Shader->bind();
        m_Shader->setFloat4("u_Color", outlineColor);
        float scale = 1.1f;
        Renderer::submit(m_Shader, vertexArray, math::scaleUniform(scale) * transform);
        
        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glEnable(GL_DEPTH_TEST);
    }
} // namespace cabrankengine::rendering
