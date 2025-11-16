#include <Cabrankengine.h>

// --- Entry Point ---
#include "Cabrankengine/Core/EntryPoint.h"

#include <Cabrankengine/Events/KeyEvent.h>
#include <Platform/OpenGL/OpenGLShader.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <Cabrankengine/Renderer/Texture.h>
#include "Sandbox2D.h"

class ExampleLayer : public cabrankengine::Layer {
public:
	ExampleLayer() : Layer("Example") {
		
	}
	
	void onUpdate(cabrankengine::Timestep delta) override {

		cabrankengine::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
		cabrankengine::RenderCommand::clear();

	}	
};

class Sandbox : public cabrankengine::Application {
public:
	Sandbox() {
		//pushLayer(new ExampleLayer());
		pushLayer(new Sandbox2D());
	}
	~Sandbox() {

	}
};

cabrankengine::Application* cabrankengine::createApplication() {
	return new Sandbox();
}
