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
#include <Cabrankengine/Renderer/TextRenderer.h>

class ExampleLayer : public cabrankengine::Layer {
public:
	ExampleLayer() : Layer("Example") {
		

		m_TextRenderer = new cabrankengine::TextRenderer(1600, 900);
		m_TextRenderer->load("assets/fonts/ocraext.ttf", 24);
	}
	
	void onUpdate(cabrankengine::Timestep delta) override {

		cabrankengine::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
		cabrankengine::RenderCommand::clear();

		//m_TextRenderer->renderText("ESTO ES UN TEXTO DE PRUEBA", 320.0f, 450.0f, 1.f, glm::vec3(0.0f, 1.0f, 1.0f));
	}	

private:
	cabrankengine::TextRenderer* m_TextRenderer;
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
