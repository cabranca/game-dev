#pragma once

#include <Cabrankengine.h>

class Sandbox2D : public cabrankengine::Layer {
	public:
		Sandbox2D();

		virtual void onAttach() override;
		virtual void onDetach() override;

		virtual void onUpdate(cabrankengine::Timestep delta) override;
		virtual void onImGuiRender() override;
		virtual void onEvent(cabrankengine::Event& e) override;

	private:
		cabrankengine::OrthographicCameraController m_CameraController;

		// Temp
		cabrankengine::Ref<cabrankengine::VertexArray> m_SquareVA;
		cabrankengine::Ref<cabrankengine::Shader> m_FlatColorShader;

		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};
