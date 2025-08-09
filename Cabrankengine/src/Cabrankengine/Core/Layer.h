#pragma once

#include <Cabrankengine/Core/Core.h>
#include <Cabrankengine/Events/Event.h>
#include <Cabrankengine/Core/Timestep.h>

namespace cabrankengine {
	
	class Layer {
	public:
		Layer(const std::string& name = "Layer"); // TODO: Make explicit?
		virtual ~Layer();

		// Callback for when the layer is pushed to the stack
		virtual void onAttach() {}

		// Callback for when the layer is popped from the stack
		virtual void onDettach() {}

		// Callback for when the layer must update
		virtual void onUpdate(Timestep delta) {}

		virtual void onImGuiRender() {}

		// Callback for the event system
		virtual void onEvent(Event& event) {}

		// Returns the layer name (mostly for debug)
		const std::string& getName() const { return m_DebugName; }

	protected:
		std::string m_DebugName; // Name of the layer (mostly for debug)
	};
}

