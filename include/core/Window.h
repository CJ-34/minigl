#pragma once
#include <GLFW/glfw3.h>


namespace minigl {
	class Window {
		public:
			Window(int w, int h, const char* title);
			~Window();
			bool shouldClose() const;
			void poll();
			void swap();
			GLFWwindow* handle() { return m_win; }
			float deltaSeconds() const { return m_dt;  }
			float width() const { return m_w; }
			float height() const { return m_h; }
		private:
			GLFWwindow* m_win{};
			int m_w{}, m_h{};
			double m_lastTime{};
			float m_dt{};
	};

}