#include "core/Window.h"
#include "core/Shader.h"
#include <spdlog/spdlog.h>
#include <iostream>

int main() {
	minigl::Window window{800, 600, "demo"};

	spdlog::info("Succesfully created window");

	minigl::Shader shader{ "assets/shaders/vert.glsl", "assets/shaders/frag.glsl" };



	while (!window.shouldClose()) {
		window.poll();

		window.swap();
	}
	

	return 0;
}