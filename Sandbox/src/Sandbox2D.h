#pragma once

#include <Cabrankengine.h>

class Sandbox2D : public cbk::Layer {
	public:
		Sandbox2D();

		virtual void onAttach() override;
		virtual void onDetach() override;

		virtual void onUpdate(cbk::Timestep delta) override;
		virtual void onImGuiRender() override;
		virtual void onEvent(cbk::Event& e) override;

	private:
		cbk::scene::OrthographicCamera m_Camera;

		// Temp
		cbk::Ref<cbk::rendering::VertexArray> m_SquareVA;

		cbk::Ref<cbk::rendering::Texture2D> m_CheckerboardTexture;
		cbk::Ref<cbk::rendering::Texture2D> m_ConvertedTexture;

		cbk::math::Vector4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
		cbk::math::Vector4 m_SquareColorVariant = { 0.8f, 0.3f, 0.2f, 1.0f };

		cbk::ecs::Registry* m_Registry;
		uint32_t m_Player;
};
