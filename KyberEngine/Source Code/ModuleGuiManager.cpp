#include "ModuleGuiManager.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleGuiManager::ModuleGuiManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleGuiManager::~ModuleGuiManager()
{}

bool ModuleGuiManager::Start()
{
	LOG("Creating GUI");
	bool ret = true;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	// window is the SDL_Window*
	// context is the SDL_GLContext
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init("#version 460");

	return ret;
}

update_status ModuleGuiManager::PreUpdate(float dt)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

update_status ModuleGuiManager::Update(float dt)
{
	update_status status;

	ImGui::ShowDemoWindow();
	ImGui::Begin("Program Options");
	if (ImGui::Button("Close App", ImVec2(75.0f, 20.0f)))
	{
		return UPDATE_ERROR;
	}
	ImGui::End();

	return UPDATE_CONTINUE;
}

update_status ModuleGuiManager::PostUpdate(float dt)
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}

bool ModuleGuiManager::CleanUp()
{
	LOG("Destroying GUI");

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}