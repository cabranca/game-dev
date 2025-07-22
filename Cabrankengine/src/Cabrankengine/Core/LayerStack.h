#pragma once

#include <Cabrankengine/Core/Core.h>
#include <Cabrankengine/Core/Layer.h>
#include <vector>

namespace cabrankengine {
	
	class CABRANKENGINE_API LayerStack {
	public:
		LayerStack();
		~LayerStack();

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);
		void popLayer(Layer* layer);
		void popOverlay(Layer* overlay);

		// TODO: actually understand this please!
		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }

	private:
		std::vector<Layer*> m_Layers;
		std::vector<Layer*>::iterator m_LayerInsert;
	};
}
