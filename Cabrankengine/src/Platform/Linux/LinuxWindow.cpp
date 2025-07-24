#include "LinuxWindow.h"
#include <Cabrankengine/Core/Logger.h>
#include <Cabrankengine/Events/MouseEvent.h>
#include <Cabrankengine/Events/ApplicationEvent.h>
#include <Cabrankengine/Events/KeyEvent.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace cabrankengine {
    static bool s_GLFWInitialized = false;

    Window* Window::create(const WindowProps& props) {
        return new LinuxWindow(props);
    }

    LinuxWindow::LinuxWindow(const WindowProps& props) {
        init(props);
    }

    LinuxWindow::~LinuxWindow() {
        shutdown();
    }

    void LinuxWindow::onUpdate() {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

    void LinuxWindow::setVSync(bool enabled) {
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);
        m_Data.VSync = enabled;
    }

    bool LinuxWindow::isVSync() const {
        return m_Data.VSync;
    }

    void LinuxWindow::init(const WindowProps& props) {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        CE_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

        if (!s_GLFWInitialized) {
            int success = glfwInit();
            CE_CORE_ASSERT(success, "Could not initialize GLFW!");
            s_GLFWInitialized = true;
        }

        m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);

        int gladLoaded = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CE_CORE_ASSERT(status, "Failed to initialize Glad!");

        glfwSetWindowUserPointer(m_Window, &m_Data);
        setVSync(true);

        // Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window); // Cast from void* to WindowData*
			WindowResizeEvent event(width, height);
			data.EventCallback(event);
			data.Width = width;
			data.Height = height;
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
				case GLFW_PRESS: {
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE: {
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT: {
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
			}
        });
    }

    void LinuxWindow::shutdown() {
        glfwDestroyWindow(m_Window);
    }

}
