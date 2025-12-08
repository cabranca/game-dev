#include <Cabrankengine.h>

// --- Entry Point ---
#include "Cabrankengine/Core/EntryPoint.h"

#include <Cabrankengine/Events/KeyEvent.h>
#include <Platform/OpenGL/OpenGLShader.h>
#include <imgui.h>
#include "Sandbox2D.h"

using namespace cabrankengine;
using namespace cabrankengine::math;
using namespace cabrankengine::rendering;
using namespace cabrankengine::platform::opengl;

class ExampleLayer : public Layer {
  public:
	ExampleLayer() : Layer("Example"), m_CameraController(PerspectiveCamera(PI / 4.f, 16.f / 9.f, 0.1f, 100.f)) {
		m_CameraController.getCamera().setWorldPosition(Vector3(0.f, 0.f, 10.f));
		auto modelTestShader = m_ShaderLibrary.load("assets/shaders/ModelTest.glsl");
		m_ModelTest = new Model("assets/models/backpack/backpack.obj");
	}

	void onUpdate(Timestep delta) override {

		RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
		RenderCommand::clear();

		m_CameraController.onUpdate(delta);

		Renderer::beginScene(m_CameraController.getCamera());


		auto shader = m_ShaderLibrary.get("ModelTest");
		m_ModelTest->draw(shader);
		
		Renderer::endScene();
	}

  private:

	CameraController m_CameraController;
	ShaderLibrary m_ShaderLibrary;
	Model* m_ModelTest = nullptr;
};

class Sandbox : public Application {
  public:
	Sandbox() {
		pushLayer(new ExampleLayer());
		//pushLayer(new Sandbox2D());
	}
	~Sandbox() {}
};

Application* cabrankengine::createApplication() {
	return new Sandbox();
}
