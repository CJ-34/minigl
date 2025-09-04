cmake_minimum_required(VERSION 3.8)

project(minigl LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# options
option(MINIGL_BUILD_DEMO "Build demo app" ON)
option(MINIGL_VENDOR_GLAD "Use vendored glad files in external/glad" ON)

include(FetchContent)

# spdlog
FetchContent_Declare(
	spdlog
	GIT_REPOSITORY https://github.com/gabime/spdlog.git
	GIT_TAG v1.15.3
)
FetchContent_MakeAvailable(spdlog)

# GLFW
FetchContent_Declare(
	glfw
	GIT_REPOSITORY	https://github.com/glfw/glfw.git
	GIT_TAG			3.4
)
FetchContent_MakeAvailable(glfw)

# glm
FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG			1.0.1
)
FetchContent_MakeAvailable(glm)

# stb_image (header-only)
FetchContent_Declare(
	stb
	GIT_REPOSITORY	https://github.com/nothings/stb.git
	GIT_TAG			master
)
FetchContent_MakeAvailable(stb)

# glad (vendored files expected at external/glad/)
if (MINIGL_VENDOR_GLAD)
	add_library(glad STATIC external/glad/src/glad.c)
	target_include_directories(glad PUBLIC external/glad/include)
endif()

# Library
file(GLOB MINIGL_HEADERS CONFIGURE_DEPENDS "include/**/*.h")
file(GLOB MINIGL_SOURCES CONFIGURE_DEPENDS "src/**/*.cpp")
add_library(minigl STATIC ${MINIGL_HEADERS} ${MINIGL_SOURCES})
target_include_directories(minigl PUBLIC "include/")
target_link_libraries(minigl PUBLIC glfw glm::glm glad spdlog)
target_compile_definitions(minigl PRIVATE STB_IMAGE_IMPLEMENTATION)
target_include_directories(minigl PRIVATE ${stb_SOURCE_DIR})

# Demo
# if(MINIGL_BULD_DEMO)
	add_executable(minigl_demo "src/main_demo.cpp" "include/core/Shader.h")
	target_link_libraries(minigl_demo PRIVATE minigl)
	# macOS: link Cocoa, IOKit, CoreVideo for GLFW
	#if (APPLE)
	#	target_link_libraries(minigl PRIVATE "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
	#	target_link_libraries(minigl_demo PRIVATE "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
	# endif()
# endif()

add_custom_command(TARGET minigl_demo POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
			${CMAKE_SOURCE_DIR}/assets
			$<TARGET_FILE_DIR:minigl_demo>/assets
)