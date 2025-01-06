#pragma once

#if 1
#include <glad/glad.h>
#endif

#include <GLFW/glfw3.h>

#include <ImGui/imgui.h>
#include <ImGui/backends/imgui_impl_glfw.h>
#include <ImGui/backends/imgui_impl_opengl3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/integer.hpp>
#include <glm/gtx/transform.hpp>

// Logging
#include "Utils/Log.h"

// Standard libraries
#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <optional>
#include <random>
#include <set>
#include <span>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

inline void GLCheckError()                                                      \
{                                                                           \
    GLenum err;                                                             \
    while ((err = glGetError()) != GL_NO_ERROR)                             \
    {                                                                       \
		LOG_ERROR("OpenGL Error: {0} at {1}:{2}", err, __FILE__, __LINE__); \
	}                                                                       \
}