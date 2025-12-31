#pragma once

#include <Cabrankengine/Renderer/RendererAPI.h>

namespace MTL {
	class RenderCommandEncoder;
	class CommandBuffer;
	class RenderPipelineState;
} // namespace MTL

namespace cabrankengine::platform::metal {

	class MetalRendererAPI : public rendering::RendererAPI {
	  public:
		// Initializes the renderer API. This method should be called before any rendering operations.
		virtual void init() override;

		// Sets the color used to clear the screen.
		virtual void setClearColor(const math::Vector4& color) override;

		// Clears the screen with the previously set clear color.
		virtual void clear() override;

		// Draws the vertex array vertices in order
		virtual void draw(const Ref<rendering::VertexArray>& vertexArray) override;

		// Draws the indexed vertices from the vertex array.
		virtual void drawIndexed(const Ref<rendering::VertexArray>& vertexArray, uint32_t indexCount = 0) override;

		// Sets the viewport dimensions for rendering.
		virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		// Returns the current rendering API.
		static API getAPI() {
			return API::Metal;
		}

		static void SetCurrentPipelineState(MTL::RenderPipelineState* pipelineState);

		void endFrame() override;

	  private:
		math::Vector4 m_ClearColor = { 0.f, 0.f, 0.f, 1.f };

		// Estado global del frame actual
        // Podrían ser miembros estáticos o singleton interno si RendererAPI es único
        MTL::CommandBuffer* m_ActiveCommandBuffer = nullptr;
        MTL::RenderCommandEncoder* m_ActiveEncoder = nullptr;
		inline static MTL::RenderPipelineState* s_CurrentPipelineState = nullptr;
	};
} // namespace cabrankengine::platform::metal
