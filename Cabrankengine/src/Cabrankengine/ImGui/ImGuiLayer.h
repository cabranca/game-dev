#pragma once

#include <Cabrankengine/Core/Layer.h>

namespace cabrankengine {
	
	class CABRANKENGINE_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		// Callback for when the layer is pushed to the stack
		virtual void onAttach() override;

		// Callback for when the layer is popped from the stack
		virtual void onDettach() override;

		virtual void onImGuiRender() override;

		void begin();
		void end();

	private:
		float m_Time; // Time used to calculate delta
	};
}