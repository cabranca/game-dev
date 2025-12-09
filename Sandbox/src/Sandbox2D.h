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
		cabrankengine::scene::OrthographicCamera m_Camera;

		// Temp
		cabrankengine::Ref<cabrankengine::rendering::VertexArray> m_SquareVA;

		cabrankengine::Ref<cabrankengine::rendering::Texture2D> m_CheckerboardTexture;

		cabrankengine::math::Vector4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
		cabrankengine::math::Vector4 m_SquareColorVariant = { 0.8f, 0.3f, 0.2f, 1.0f };

		cabrankengine::ecs::Registry* m_Registry;
		uint32_t m_Player;
};
