#pragma once

#include <string>
#include <Cabrankengine/Core/Timestep.h>

namespace cabrankengine {

	class Event; // Forward declaration of Event class 
	
	// Layer class represents a single layer in the application.
	// Layers can be used to separate different parts of the application logic,
	// such as rendering, input handling, and game logic.
	// Layers can be pushed and popped from a stack, allowing for flexible management of application state.
	// Layers are executed in the order they are pushed onto the stack.
	// TODO: could this be an interface? There's no default implementation of the methods.
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

