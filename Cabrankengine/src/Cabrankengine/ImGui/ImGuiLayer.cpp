#include <Cabrankengine/ImGui/ImGuiLayer.h>

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>
#include <Cabrankengine/Core/Application.h>
#include <Cabrankengine/Core/Window.h>

namespace cabrankengine {

	ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}

	ImGuiLayer::~ImGuiLayer() {}

	void ImGuiLayer::onAttach()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			style.WindowRounding = 0.f;
			style.Colors[ImGuiCol_WindowBg].w = 1.f;
		}

		auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::onDettach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::onImGuiRender() {
		static bool show = true;
		ImGui::ShowDemoWindow(&show);
	}

	void ImGuiLayer::begin() {
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::end() {
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::get();
		io.DisplaySize = ImVec2((float)app.getWindow().getWidth(), (float)app.getWindow().getHeight()); // TODO: use static cast

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}
}