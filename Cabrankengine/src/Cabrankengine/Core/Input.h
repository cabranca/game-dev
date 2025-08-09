#pragma once

#include <Cabrankengine/Core/Core.h>
#include <Cabrankengine/Core/KeyCodes.h>
#include <Cabrankengine/Core/MouseCodes.h>
#include <tuple>

namespace cabrankengine {

	class CABRANKENGINE_API Input {
	public:
		static bool isKeyPressed(KeyCode keyCode) { return s_Instance->isKeyPressedImpl(keyCode); }

		static bool isMouseButtonPressed(MouseCode mouseCode) { return s_Instance->isMouseButtonPressedImpl(mouseCode); }
		static std::pair<float, float> getMousePosition() { return s_Instance->getMousePositionImpl(); }
		static float getMouseX() { return s_Instance->getMouseXImpl(); }
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