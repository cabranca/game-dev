#pragma once

#include <utility>
#include <Cabrankengine/Core/Core.h>
#include <Cabrankengine/Core/KeyCodes.h>
#include <Cabrankengine/Core/MouseCodes.h>

namespace cabrankengine {

	// This class is used to perform input polling.
	// This is a mix between an interface and a Pimpl pattern.
	// TODO: check if this is a good design choice.
	class Input {
		public:
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
			static Input* s_Instance; //TODO: check how to initialize inline (C++17/20 feature).
	};
}