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
		

		m_TextRenderer = new cabrankengine::TextRenderer(2560, 1440);
		m_TextRenderer->load("assets/fonts/ocraext.TTF", 12);
	}
	
	void onUpdate(cabrankengine::Timestep delta) override {

		cabrankengine::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
		cabrankengine::RenderCommand::clear();

		m_TextRenderer->renderText("ESTO ES UN TEXTO DE PRUEBA", 100.0f, 100.0f, 10.f, glm::vec3(1.0f, 1.0f, 1.0f));
	}	

private:
	cabrankengine::TextRenderer* m_TextRenderer;
};

class Sandbox : public cabrankengine::Application {
public:
	Sandbox() {
		pushLayer(new ExampleLayer());
		//pushLayer(new Sandbox2D());
	}
	~Sandbox() {

	}
};

cabrankengine::Application* cabrankengine::createApplication() {
	return new Sandbox();
}
