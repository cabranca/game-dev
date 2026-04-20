#pragma once

#include <Cabrankengine/Renderer/RendererAPI.h>

namespace MTL {
	class RenderCommandEncoder;
	class CommandBuffer;
} // namespace MTL

namespace cbk::platform::metal {

	class MetalShader; // Forward declaration

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

		// Sets the currently active Metal shader for the next draw call.
		static void SetCurrentShader(MetalShader* shader);

		// Returns the active render command encoder (used by textures to bind themselves).
		static MTL::RenderCommandEncoder* GetActiveEncoder() { return s_ActiveEncoder; }

		void endFrame() override;

	  private:
		math::Vector4 m_ClearColor = { 0.f, 0.f, 0.f, 1.f };

		// Per-frame state (static so other Metal subsystems can access them)
        MTL::CommandBuffer* m_ActiveCommandBuffer = nullptr;
        inline static MTL::RenderCommandEncoder* s_ActiveEncoder = nullptr;
		inline static MetalShader* s_CurrentShader = nullptr;
	};
} // namespace cbk::platform::metal
