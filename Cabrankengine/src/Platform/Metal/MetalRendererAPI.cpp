#include <pch.h>
#include "MetalRendererAPI.h"

#include <Cabrankengine/Core/Application.h>
#include <Cabrankengine/Core/Window.h>
#include "MetalContext.h"

#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

#include <Cabrankengine/Renderer/VertexArray.h>
#include <Cabrankengine/Renderer/Buffer.h>

namespace cabrankengine::platform::metal {

	using namespace rendering;

	void MetalRendererAPI::init() {}

	void MetalRendererAPI::setClearColor(const math::Vector4& color) {
		m_ClearColor = color;
	}

	void MetalRendererAPI::clear() {
		const auto& window = Application::get().getWindow();
		MetalContext* context = static_cast<MetalContext*>(window.getContext());

		MTL::CommandQueue* queue = context->getCommandQueue();
		CA::MetalDrawable* drawable = context->getCurrentDrawable();

		if (!drawable)
			return;

		// 3. Crear Command Buffer
		m_ActiveCommandBuffer = queue->commandBuffer();

		// 4. Configurar RenderPass (Esto es el "Clear")
		MTL::RenderPassDescriptor* pass = MTL::RenderPassDescriptor::renderPassDescriptor();
		auto colorAtt = pass->colorAttachments()->object(0);
		colorAtt->setTexture(drawable->texture());
		colorAtt->setLoadAction(MTL::LoadActionClear); // <--- La acción de borrar
		colorAtt->setClearColor(MTL::ClearColor::Make(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
		colorAtt->setStoreAction(MTL::StoreActionStore);

		// 5. Crear el Encoder (Empezamos a grabar comandos)
		m_ActiveEncoder = m_ActiveCommandBuffer->renderCommandEncoder(pass);
		pass->release();
	}

	void MetalRendererAPI::draw(const Ref<VertexArray>& vertexArray) {}

	void MetalRendererAPI::drawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount) {
		if (!m_ActiveEncoder || !s_CurrentPipelineState)
			return;

		m_ActiveEncoder->setRenderPipelineState(s_CurrentPipelineState);
		m_ActiveEncoder->drawPrimitives(MTL::PrimitiveTypeTriangle, (NS::UInteger)0, (NS::UInteger)3);
	}

	void MetalRendererAPI::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {}

	void MetalRendererAPI::SetCurrentPipelineState(MTL::RenderPipelineState* pipelineState) {
		s_CurrentPipelineState = pipelineState;
	}

	void MetalRendererAPI::endFrame() {
		if (m_ActiveEncoder) {
			m_ActiveEncoder->endEncoding();
			m_ActiveEncoder->release(); // Ojo con el manejo de memoria en metal-cpp
			m_ActiveEncoder = nullptr;
		}

		if (m_ActiveCommandBuffer) {
			// El commit real lo hace swapBuffers, pero necesitamos pasarle el buffer
			// O podemos commitear acá y swapBuffers solo presenta.
			// Lo más limpio es que Context::SwapBuffers se encargue de presentar.
			// Para eso, necesitamos que Context tenga acceso al buffer, o commitear aquí.

			// Opción simple: Commitear aquí y presentar.
			MetalContext* context = static_cast<MetalContext*>(Application::get().getWindow().getContext());
			CA::MetalDrawable* drawable = context->getCurrentDrawable();

			m_ActiveCommandBuffer->presentDrawable(drawable);
			m_ActiveCommandBuffer->commit();
			m_ActiveCommandBuffer->release();
			m_ActiveCommandBuffer = nullptr;
		}
	}
} // namespace cabrankengine::platform::metal
