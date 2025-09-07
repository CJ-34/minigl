#include <glad/glad.h>
#include "core/Window.h"
#include <spdlog/spdlog.h>

namespace minigl {
	Window::Window(int w, int h, const char* title)
		: m_w(w), m_h(h)
	{
		if (!glfwInit()) {
			spdlog::error("Failed to initialize GLFW!");
			throw std::runtime_error("GLFW init failed");
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_win = glfwCreateWindow(w, h, title, NULL, NULL);

		if (!m_win) {
			spdlog::error("Failed to create window");
			glfwTerminate();
			std::runtime_error("Window creation failed");
		}
		glfwMakeContextCurrent(m_win);

		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_win);
		glfwTerminate();
	}

	bool Window::shouldClose() const
	{
		return glfwWindowShouldClose(m_win);
	}

	void Window::poll()
	{
		glfwPollEvents();
	}

	void Window::swap()
	{
		glfwSwapBuffers(m_win);
	}
}


