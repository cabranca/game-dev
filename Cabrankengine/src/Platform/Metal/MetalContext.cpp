#include <pch.h>
#include "MetalContext.h"

#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

// Para interactuar con la ventana de GLFW (Cocoa) desde C++
#include <objc/runtime.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace cabrankengine::platform::metal {

	const char* triangleShaderSrc = R"(
        #include <metal_stdlib>
        using namespace metal;
        struct VertexOut { float4 position [[position]]; float4 color; };
        vertex VertexOut vertex_main(uint vertexID [[vertex_id]]) {
            float2 positions[3] = { {0.0, 0.5}, {0.5, -0.5}, {-0.5, -0.5} };
            float3 colors[3] = { {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0} };
            VertexOut out;
            out.position = float4(positions[vertexID], 0.0, 1.0);
            out.color = float4(colors[vertexID], 1.0);
            return out;
        }
        fragment float4 fragment_main(VertexOut in [[stage_in]]) { return in.color; }
    )";

	MetalContext::MetalContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle) {
		CE_CORE_ASSERT(m_WindowHandle, "Window handle is null!");

		m_Device = MTL::CreateSystemDefaultDevice();
	}

	MetalContext::~MetalContext() {
		if (m_PipelineState)
			m_PipelineState->release();
		if (m_Swapchain)
			m_Swapchain->release();
		if (m_CommandQueue)
			m_CommandQueue->release();
		if (m_Device)
			m_Device->release();
	}

	void MetalContext::init() {
		CE_PROFILE_FUNCTION();
		CE_CORE_INFO("Metal Context (C++) - GPU: {0}", m_Device->name()->utf8String());

		m_CommandQueue = m_Device->newCommandQueue();

		// --- MAGIA para conectar GLFW con Metal-cpp sin usar .mm ---
		// 1. Obtener la NSWindow (void*)
		void* nswindow = glfwGetCocoaWindow(m_WindowHandle);

		// 2. Obtener la contentView: [nswindow contentView]
		// Usamos objc_msgSend para llamar métodos de ObjC desde C++
		void* contentView = ((void* (*)(id, SEL))objc_msgSend)((id)nswindow, sel_registerName("contentView"));

		// 3. Configurar el Layer
		m_Swapchain = CA::MetalLayer::layer();
		m_Swapchain->setDevice(m_Device);
		m_Swapchain->setPixelFormat(MTL::PixelFormatBGRA8Unorm);

		// 4. Asignar el layer: [view setLayer:layer] y [view setWantsLayer:YES]
		((void (*)(id, SEL, void*))objc_msgSend)((id)contentView, sel_registerName("setLayer:"), (void*)m_Swapchain);
		((void (*)(id, SEL, BOOL))objc_msgSend)((id)contentView, sel_registerName("setWantsLayer:"), YES);
		// ------------------------------------------------------------

		// Compilar Shader
		NS::Error* error = nullptr;
		NS::String* src = NS::String::string(triangleShaderSrc, NS::StringEncoding::UTF8StringEncoding);

		MTL::Library* library = m_Device->newLibrary(src, nullptr, &error);
		if (!library) {
			CE_CORE_ERROR("Error shader: {0}", error->localizedDescription()->utf8String());
			return;
		}

		MTL::Function* vertFunc = library->newFunction(NS::String::string("vertex_main", NS::UTF8StringEncoding));
		MTL::Function* fragFunc = library->newFunction(NS::String::string("fragment_main", NS::UTF8StringEncoding));

		MTL::RenderPipelineDescriptor* desc = MTL::RenderPipelineDescriptor::alloc()->init();
		desc->setVertexFunction(vertFunc);
		desc->setFragmentFunction(fragFunc);
		desc->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormatBGRA8Unorm);

		m_PipelineState = m_Device->newRenderPipelineState(desc, &error);
		if (!m_PipelineState)
			CE_CORE_ERROR("Error pipeline: {}", error->localizedDescription()->utf8String());

		desc->release();
		vertFunc->release();
		fragFunc->release();
		library->release();
	}

	void MetalContext::swapBuffers() {
		CA::MetalDrawable* drawable = m_Swapchain->nextDrawable();
		if (!drawable)
			return;

		MTL::CommandBuffer* cmd = m_CommandQueue->commandBuffer();
		MTL::RenderPassDescriptor* pass = MTL::RenderPassDescriptor::renderPassDescriptor();

		auto colorAtt = pass->colorAttachments()->object(0);
		colorAtt->setTexture(drawable->texture());
		colorAtt->setLoadAction(MTL::LoadActionClear);
		colorAtt->setClearColor(MTL::ClearColor::Make(0.1, 0.1, 0.1, 1.0));
		colorAtt->setStoreAction(MTL::StoreActionStore);

		MTL::RenderCommandEncoder* enc = cmd->renderCommandEncoder(pass);
		if (m_PipelineState) {
			enc->setRenderPipelineState(m_PipelineState);
			enc->drawPrimitives(MTL::PrimitiveTypeTriangle, (NS::UInteger)0, (NS::UInteger)3);
		}
		enc->endEncoding();

		cmd->presentDrawable(drawable);
		cmd->commit();

		// metal-cpp usa autorelease pools internamente para algunos objetos,
		// pero aquí los objetos pass y enc son temporales.
	}
} // namespace cabrankengine::platform::metal
