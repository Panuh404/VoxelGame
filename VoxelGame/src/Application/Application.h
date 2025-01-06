#pragma once
#include "Gui.h"
#include "ImGui.h"
#include "Window.h"
#include "Scene/Scene.h"
#include "Scene/AssetManager/AssetManager.h"

class Application
{
	static Application *instancePtr;

	using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;
	using Clock = std::chrono::steady_clock;

	Window window;
	GUI gui;
	AssetManager assetManager;
	std::shared_ptr<Scene> scene;
	TimePoint lastTick = Clock::now();

	void UpdateAndRender();
	void OnRefreshWindow();
	void OnKeyEvent(int32_t key, int32_t scancode, int32_t action, int32_t mode);
	void OnMouseButtonEvent(int32_t button, int32_t action, int32_t mods);
	void OnCursorPositionEvent(double x, double y);
	void OnResized(int32_t width, int32_t height);

	friend Window;

public:
	Application();
	~Application();

	static Application &instance() { return *instancePtr; }

	void SetScene(const std::shared_ptr<Scene> &newScene) { scene = newScene; }
	int32_t GetWindowWidth() { return window.GetWindowWidth(); }
	int32_t GetWindowHeight() { return window.GetWindowHeight(); }
	Window &GetWindow() { return window; }
	int32_t Run();
};
