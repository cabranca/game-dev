#pragma once

#include <Cabrankengine.h>

class Sandbox2D : public cbk::Layer {
	public:
		Sandbox2D(const cbk::Ref<cbk::ecs::Registry>& reg);

		virtual void onAttach() override;
		virtual void onDetach() override;

		virtual void onUpdate(cbk::Timestep delta) override;
		virtual void onImGuiRender() override;
		virtual void onEvent(cbk::Event& e) override;

	private:
		cbk::scene::OrthographicCamera m_Camera;
		cbk::Ref<cbk::rendering::Texture2D> m_ConvertedTexture;
		cbk::Ref<cbk::ecs::Registry> m_Registry;
};
