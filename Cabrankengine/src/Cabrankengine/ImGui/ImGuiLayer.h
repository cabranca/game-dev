#pragma once

#include <Cabrankengine/Core/Layer.h>

namespace cabrankengine {
	
	class CABRANKENGINE_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		// Callback for when the layer is pushed to the stack
		virtual void onAttach() override; // TODO: Cherno didn't make this virtual override.

		// Callback for when the layer is popped from the stack
		virtual void onDettach() override;

		// Callback for when the layer must update
		virtual void onUpdate() override;

		// Callback for the event system
		virtual void onEvent(Event& event) override;

	private:
		float m_Time; // Time used to calculate delta
	};
}