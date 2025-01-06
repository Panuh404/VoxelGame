#pragma once
#include "GameSettings.h"
#include "Rendering/FramebufferStack.h"

class Window
{
	static Window *instancePtr;

	const char *name = Settings::WindowName;
	int32_t windowWidth = Settings::WindowWidth;
	int32_t windowHeight = Settings::WindowHeight;
	GLFWwindow *window = nullptr;
	glm::vec4 clearColor = { 0,0,0,1 };
	std::shared_ptr<FramebufferStack> framebufferStack = std::make_shared<FramebufferStack>();

	void SetupCallbacks();
	static bool SetupGlad();

	static void OnKeyEvent(GLFWwindow *window, int32_t key, int32_t scancode, int32_t action, int32_t mode);
	static void OnResized(GLFWwindow *window, int32_t width, int32_t height);
	static void OnMouseButtonEvent(GLFWwindow *window, int32_t button, int32_t action, int32_t mods);
	static void OnCursorPosition(GLFWwindow *window, double x, double y);
	static void OnRefreshWindow(GLFWwindow *window);
	static void OnWindowError(int32_t errorCode, const char *description);
	static void OnOpenGlMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);

public:
	Window();
	~Window();

	static Window &instance() { return *instancePtr; }

	GLFWwindow *GetContext() { return window; }
	std::shared_ptr<FramebufferStack> GetFramebufferStack() { return framebufferStack; }

	int32_t GetWindowWidth() const { return windowWidth; }
	int32_t GetWindowHeight() const { return windowHeight; }
	void SetWindowWidth(int32_t width) { windowWidth = width; }
	void SetWindowHeight(int32_t height) { windowHeight = height; }

	bool IsValid() { return window != nullptr; }
	bool ShouldClose() const { return glfwWindowShouldClose(window); }

	// Handle frame
	void BeginFrame();
	void ResetFrame();
	void EndFrame();
	void SwapBuffers();
	bool ShouldRender();

	void PollEvents();
	void UnlockMouse();
	void LockMouse();
	glm::dvec2 GetCursorPosition();
};
