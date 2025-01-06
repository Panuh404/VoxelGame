#include "GamePCH.h"
#include "Window.h"

#include "Application.h"
#include "Rendering/ColorRenderPass.h"



Window *Window::instancePtr = nullptr;

Window::Window()
{
	assert(instancePtr == nullptr && "Window already instantiated");
	instancePtr = this;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#ifndef BUILD_TYPE_DIST
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	#endif
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	window = glfwCreateWindow(windowWidth, windowHeight, name, nullptr, nullptr);
	glfwMakeContextCurrent(window);

	
	if (window == nullptr)
	{
		LOG_ERROR("Failed to create GLFW window");
		return;
	}
	
	if (!SetupGlad())
	{
		LOG_ERROR("Failed to initialize OpenGL context");
		window = nullptr;
		return;
	}
	SetupCallbacks();
}

Window::~Window()
{
	instancePtr = nullptr;
	glfwTerminate();
}

void Window::BeginFrame()
{
	assert(framebufferStack->Empty());
	ResetFrame();

	static std::shared_ptr<Framebuffer> framebuffer = nullptr;
	if (framebuffer == nullptr || framebuffer->GetWidth() != windowWidth || framebuffer->GetHeight() != windowHeight) 
	{
		framebuffer = std::make_shared<Framebuffer>(windowWidth, windowHeight, true, 1);
	}
	framebufferStack->Push(framebuffer);
	ResetFrame();
}

void Window::ResetFrame()
{
	glViewport(0, 0, windowWidth, windowHeight);
	glClearColor(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Window::EndFrame()
{
	assert(framebufferStack->Size() == 1);
	ColorRenderPass::RenderTexture(framebufferStack->Pop()->GetColorAttachment(0));
}

void Window::SwapBuffers()
{
	framebufferStack->ClearIntermediateTextureReferences();
	glfwSwapBuffers(window);
}

bool Window::ShouldRender()
{
	return windowWidth > 0 && windowHeight > 0;
}

void Window::PollEvents()
{
	glfwPollEvents();
	GLCheckError();
}

void Window::UnlockMouse()
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	GLCheckError();
}

void Window::LockMouse()
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	GLCheckError();
}

glm::dvec2 Window::GetCursorPosition()
{
	glm::dvec2 pos;
	glfwGetCursorPos(window, &pos.x, &pos.y);
	GLCheckError();
	return pos;
}

void Window::SetupCallbacks()
{
	glfwSetKeyCallback(window, OnKeyEvent);
	glfwSetMouseButtonCallback(window, OnMouseButtonEvent);
	glfwSetCursorPosCallback(window, OnCursorPosition);
	glfwSetFramebufferSizeCallback(window, OnResized);

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(OnOpenGlMessage, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	glfwSwapInterval(1);

	glfwSetWindowRefreshCallback(window, OnRefreshWindow);
	glfwSetErrorCallback(OnWindowError);
	GLCheckError();
}

bool Window::SetupGlad()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LOG_ERROR("Failed to initialize GLAD");
		return false;
	}
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	return true;
}

void Window::OnKeyEvent(GLFWwindow*, int32_t key, int32_t scancode, int32_t action, int32_t mode)
{
	Application::instance().OnKeyEvent(key, scancode, action, mode);
}

void Window::OnResized(GLFWwindow*, int32_t width, int32_t height)
{
	Application &app = Application::instance();
	Window &window = app.GetWindow();
	window.SetWindowWidth(width);
	window.SetWindowHeight(height);
	app.OnResized(width, height);
}

void Window::OnMouseButtonEvent(GLFWwindow*, int32_t button, int32_t action, int32_t mods)
{
	Application::instance().OnMouseButtonEvent(button, action, mods);
}

void Window::OnCursorPosition(GLFWwindow*, double x, double y)
{
	Application::instance().OnCursorPositionEvent(x, y);
}

void Window::OnRefreshWindow(GLFWwindow*)
{
	Application::instance().OnRefreshWindow();
}

void Window::OnWindowError(int32_t errorCode, const char* description)
{
	LOG_WARN("GLFW: error={0} description={1}", errorCode, description);
}

void Window::OnOpenGlMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	if (id == 131185 || id == 131218 || id == 131169 || id == 131076 || id == 131204) return;

	LOG_TRACE("DEBUG MESSAGE ({0}): {1}", id, message);

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:				LOG_TRACE("Source: API"); break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		LOG_TRACE("Source: Window System"); break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:	LOG_TRACE("Source: Shader Compiler"); break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:		LOG_TRACE("Source: Third Party"); break;
	case GL_DEBUG_SOURCE_APPLICATION:		LOG_TRACE("Source: Application"); break;
	case GL_DEBUG_SOURCE_OTHER:				LOG_TRACE("Source: Other"); break;
	default: break;
	}

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:					LOG_TRACE("Type: Error"); break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:		LOG_TRACE("Type: Deprecated Behaviour"); break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:		LOG_TRACE("Type: Undefined Behaviour"); break;
	case GL_DEBUG_TYPE_PORTABILITY:				LOG_TRACE("Type: Portability"); break;
	case GL_DEBUG_TYPE_PERFORMANCE:				LOG_TRACE("Type: Performance"); break;
	case GL_DEBUG_TYPE_MARKER:					LOG_TRACE("Type: Marker"); break;
	case GL_DEBUG_TYPE_PUSH_GROUP:				LOG_TRACE("Type: Push Group"); break;
	case GL_DEBUG_TYPE_POP_GROUP:				LOG_TRACE("Type: Pop Group"); break;
	case GL_DEBUG_TYPE_OTHER:					LOG_TRACE("Type: Other"); break;
	default: break;
	}

	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:				LOG_TRACE("Severity: high"); break;
	case GL_DEBUG_SEVERITY_MEDIUM:				LOG_TRACE("Severity: medium"); break;
	case GL_DEBUG_SEVERITY_LOW:					LOG_TRACE("Severity: low"); break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:		LOG_TRACE("Severity: notification"); break;
	default: break;
	}
}
