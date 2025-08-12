#pragma once

#include <utility>
#include <Cabrankengine/Core/Core.h>
#include <Cabrankengine/Core/KeyCodes.h>
#include <Cabrankengine/Core/MouseCodes.h>

namespace cabrankengine {

	// This class is used to perform input polling.
	// On one hand, the class is abstract so it can be implemented for different platforms (e.g., Windows, Linux, etc.) such as the renderer related classes.
	// On the other hand, it is a singleton so that we can access it from anywhere in the code.
	// As we need to access it from anywhere, the methods are static. That's why there is an indirection through a static instance pointer.
	class Input {
		public:
			virtual ~Input() = default;

			// Returns whether a certain key is pressed.
			static bool isKeyPressed(KeyCode keyCode) { return s_Instance->isKeyPressedImpl(keyCode); }

			// Returns whether a certain mouse button is pressed.
			static bool isMouseButtonPressed(MouseCode mouseCode) { return s_Instance->isMouseButtonPressedImpl(mouseCode); }

			// Returns the current mouse position as a pair of floats (x, y).
			static std::pair<float, float> getMousePosition() { return s_Instance->getMousePositionImpl(); }

			// Returns the current mouse X position.
			static float getMouseX() { return s_Instance->getMouseXImpl(); }

			// Returns the current mouse Y position.
			static float getMouseY() { return s_Instance->getMouseYImpl(); }

		protected:
			virtual bool isKeyPressedImpl(KeyCode keyCode) = 0;
			virtual bool isMouseButtonPressedImpl(MouseCode mouseCode) = 0;
			virtual std::pair<float, float> getMousePositionImpl() = 0;
			virtual float getMouseXImpl() = 0;
			virtual float getMouseYImpl() = 0;

		private:
			// This could be initialized inline but it is initialized in the platform specific implementation file.
			// This is not the approach for the renderer but it could be.
			static Input* s_Instance;
	};
}