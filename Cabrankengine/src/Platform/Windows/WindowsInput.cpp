#include <Platform/Windows/WindowsInput.h>
#include <Cabrankengine/Core/Application.h>
#include <Cabrankengine/Core/Window.h>
#include <GLFW/glfw3.h>

namespace cabrankengine {

	Input* Input::s_Instance = new WindowsInput(); //TODO: who destroys this???

	bool WindowsInput::isKeyPressedImpl(KeyCode keyCode) {
		auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		int state = glfwGetKey(window, keyCode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::isMouseButtonPressedImpl(MouseCode mouseCode) {
		auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		int state = glfwGetMouseButton(window, mouseCode);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::getMousePositionImpl() {
		auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		return { static_cast<float>(xPos), static_cast<float>(yPos) }; //TODO: change every c-style cast to static_cast
	}

	float WindowsInput::getMouseXImpl() {
		auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		return static_cast<float>(xPos);
	}

	float WindowsInput::getMouseYImpl() {
		auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		return static_cast<float>(yPos);
	}
}