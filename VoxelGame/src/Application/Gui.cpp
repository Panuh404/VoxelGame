#include "GamePCH.h"
#include "Gui.h"

#include "Window.h"

GUI *GUI::instancePtr = nullptr;

GUI::GUI()
{
	assert(instancePtr == nullptr && "The GUI is already instantiated");
	instancePtr = this;

	const auto context = Window::instance().GetContext();
	if (context == nullptr) return;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(context, true);
	ImGui_ImplOpenGL3_Init("#version 450 core");
}

GUI::~GUI()
{
	instancePtr = nullptr;
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void GUI::BeginFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void GUI::EndFrame()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}