#include "GamePCH.h"
#include "Application.h"

Application* Application::instancePtr = nullptr;

Application::Application()
{
	assert(instancePtr == nullptr && "The application is already instantiated");
	Log::Init();
	LOG_INFO("OpenGL Version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
	LOG_INFO("OpenGL Vendor: {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
	LOG_INFO("OpenGL Renderer: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
	LOG_INFO("GLSL Version: {}", reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));

	instancePtr = this;
}

Application::~Application()
{
	instancePtr = nullptr;
}

int32_t Application::Run()
{
	if (!scene || !window.IsValid()) 
	{
		return -1;
	}

	LOG_TRACE("Application Run");
	lastTick = Clock::now();
	while (!window.ShouldClose()) 
	{
		window.PollEvents();
		UpdateAndRender();
	}
	return 0;
}

void Application::UpdateAndRender()
{
	TimePoint now = Clock::now();
	float deltaTime = static_cast<float>((now - lastTick).count()) / 1000000000.0f;
	lastTick = now;

	scene->Update(deltaTime);

	if (window.ShouldRender())
	{
		window.BeginFrame();
		scene->Render();
		window.EndFrame();

		gui.BeginFrame();
		scene->RenderGUI();
		gui.EndFrame();

		window.SwapBuffers();
	}
}

void Application::OnRefreshWindow()
{
	UpdateAndRender();
}

void Application::OnKeyEvent(int32_t key, int32_t scancode, int32_t action, int32_t mode)
{
	scene->OnKeyEvent(key, scancode, action, mode);
}

void Application::OnMouseButtonEvent(int32_t button, int32_t action, int32_t mods)
{
	scene->OnMouseButtonEvent(button, action, mods);
}

void Application::OnCursorPositionEvent(double x, double y)
{
	scene->OnCursorPositionEvent(x, y);
}

void Application::OnResized(int32_t width, int32_t height)
{
	scene->OnResized(width, height);
}