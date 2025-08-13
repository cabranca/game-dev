#pragma once

#include <Cabrankengine/Core/Layer.h>

namespace cabrankengine {
	
	// ImGuiLayer is a layer that integrates ImGui into the Cabrankengine framework.
	// Mainly used for debugging and UI purposes.
	class ImGuiLayer : public Layer {
		public:
			ImGuiLayer();
			~ImGuiLayer() = default;

			// Callback for when the layer is pushed to the stack
			virtual void onAttach() override;

			// Callback for when the layer is popped from the stack
			virtual void onDetach() override;

			// Callback for when it's time to render the ImGui layer
			virtual void onImGuiRender() override;

			// Initialize backends for ImGui rendering
			void begin();

			// Render and end the ImGui frame
			void end();
	};
}