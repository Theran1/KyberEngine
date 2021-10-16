#include "ModuleGuiManager.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleGuiManager::ModuleGuiManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	// ImGui booleans ---
	tabAbout = nullptr;
	tabConfig = nullptr;
}

ModuleGuiManager::~ModuleGuiManager()
{
	for (std::vector<Tab*>::reverse_iterator it = listTabs.rbegin(); it != listTabs.rend(); ++it)
	{
		RELEASE(*it);
	}
}

bool ModuleGuiManager::Start()
{
	LOG("Creating GUI");
	bool ret = true;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init();

	tabAbout = new TabAbout(App);
	tabConfig = new TabConfig(App);

	AddTab(tabAbout);
	AddTab(tabConfig);

	return ret;
}

update_status ModuleGuiManager::PreUpdate(float dt)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleGuiManager::Update(float dt)
{
	update_status status = update_status::UPDATE_CONTINUE;

	//ImGui::ShowDemoWindow();

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			if (ImGui::MenuItem("Quit Kyber Engine"))
				return status = update_status::UPDATE_STOP;

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Settings"))
		{
			ImGui::MenuItem("Options", NULL, &tabConfig->active);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			ImGui::MenuItem("About", NULL, &tabAbout->active);
			ImGui::EndMenu();
		}
	}
	ImGui::EndMainMenuBar();

	for (std::vector<Tab*>::iterator it = listTabs.begin(); it != listTabs.end() && status == update_status::UPDATE_CONTINUE; ++it)
	{
		if ((*it)->active)
			status = (*it)->Update();
	}

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleGuiManager::PostUpdate(float dt)
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return update_status::UPDATE_CONTINUE;
}

bool ModuleGuiManager::CleanUp()
{
	LOG("Destroying GUI");

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

void ModuleGuiManager::AddTab(Tab* tab)
{
	listTabs.push_back(tab);
}