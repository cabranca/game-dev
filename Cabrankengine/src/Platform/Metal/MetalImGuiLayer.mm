#include <Cabrankengine/ImGui/ImGuiLayer.h>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_metal.h>

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>

#include "Cabrankengine/Core/Application.h"

namespace cbk {

    ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}

    void ImGuiLayer::onAttach()
    {
        // 1. Setup Context
        // IMGUI_CHECKVERSION();
        // ImGui::CreateContext();
        // ImGuiIO& io = ImGui::GetIO();
        // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        // ImGui::StyleColorsDark();

        // 2. Setup Platform/Renderer backends
        // Application& app = Application::get();
        // GLFWwindow* window = static_cast<GLFWwindow*>(app.getWindow().getNativeWindow());

        // // Importante: InstallCallbacks = true
        // ImGui_ImplGlfw_InitForOther(window, true);
        
        // // Magia de Metal: Necesitamos el MTLDevice
        // // Asumiendo que GLFW ya creó el device internamente al pedir contexto no-api
        // // O podés obtenerlo: id<MTLDevice> device = MTLCreateSystemDefaultDevice();
        // // NOTA: Lo ideal es sacarlo de tu GraphicsContext si ya lo creaste.
        // // Por simplicidad acá creamos uno default si no lo tenés a mano:
        // id<MTLDevice> device = MTLCreateSystemDefaultDevice();
        // ImGui_ImplMetal_Init(device);
    }

    void ImGuiLayer::onDetach()
    {
        // ImGui_ImplMetal_Shutdown();
        // ImGui_ImplGlfw_Shutdown();
        // ImGui::DestroyContext();
    }

    void ImGuiLayer::begin()
    {
        // ImGui_ImplMetal_NewFrame([MTLRenderPassDescriptor renderPassDescriptor]);
        // ImGui_ImplGlfw_NewFrame();
        // ImGui::NewFrame();
    }

    void ImGuiLayer::end()
    {
        //ImGui::Render();
        
        // ACÁ hay un truco: ImGui Metal necesita un RenderEncoder activo.
        // Normalmente esto se hace en el Renderer principal.
        // Si tu Renderer::EndScene() ya commiteó el buffer, esto no va a andar.
        // Necesitás inyectar el dibujo de ImGui antes de terminar el frame de Metal.
        
        // ... Lógica de draw data ...
        // ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), commandBuffer, renderEncoder);
    }
}