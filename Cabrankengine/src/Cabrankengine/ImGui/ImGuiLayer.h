#pragma once

#include <Cabrankengine/Core/Layer.h>

namespace cabrankengine {
	
	class CABRANKENGINE_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void onAttach() override; // TODO: Cherno didn't make this virtual override.
		virtual void onDettach() override;
		virtual void onUpdate() override;
		virtual void onEvent(Event& event) override;

	private:
		float m_Time;
	};
}