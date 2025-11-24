#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

#include <Cabrankengine/Debug/Instrumentator.h>
#include <Cabrankengine/Renderer/VertexArray.h>
#include <Cabrankengine/Renderer/Buffer.h>

namespace cabrankengine::platform::opengl {

	using namespace rendering;

	void OpenGLRendererAPI::init() {
		CE_PROFILE_FUNCTION();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	}

	void OpenGLRendererAPI::setClearColor(const math::Vector4& color) {
		glClearColor(color.x, color.y, color.z, color.w);
	}

	void OpenGLRendererAPI::clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::draw(const Ref<VertexArray>& vertexArray)
	{
		auto a = vertexArray->getVertexBuffers()[0]->getLayout().getElements().size();
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void OpenGLRendererAPI::drawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount) {
		uint32_t count = indexCount == 0 ? vertexArray->getIndexBuffer()->getCount() : indexCount;
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
		glViewport(x, y, width, height);
	}
}
