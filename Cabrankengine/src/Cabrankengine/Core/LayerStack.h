#pragma once

#include <Cabrankengine/Core/Layer.h>
#include <vector>

namespace cabrankengine {
	
	class LayerStack {
	public:
		LayerStack();
		~LayerStack();

		// Pushes a layer to the stack on top of the other layers (always under the overlays)
		void pushLayer(Layer* layer);

		// Pushes a layer to the stack on top of the other overlays
		void pushOverlay(Layer* overlay);

		// Pops the top layer from the stack
		void popLayer(Layer* layer);

		// Pops the top overlay from the stack
		void popOverlay(Layer* overlay);

		// TODO: actually understand this please!
		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }

	private:
		std::vector<Layer*> m_Layers; // Actual structure for the layers
		unsigned int m_LayerInsertIndex; // Custom iterator for the layers. The idea is to always push a new layer on top of other layers but under every overlay. For the overlays, push them always on top of everything
	};
}
