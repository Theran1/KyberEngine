#include "ModuleGuiManager.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleGuiManager::ModuleGuiManager(Application* app, bool start_enabled) : Module(app, start_enabled), fpsLog(FPS_LOG_SIZE), frametimeLog(FPS_LOG_SIZE)
{
	// ImGui booleans ---
	show_configWindow = false;
	show_aboutWindow = false;
}

ModuleGuiManager::~ModuleGuiManager()
{}

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

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			if (ImGui::MenuItem("Quit Kyber Engine"))
				return UPDATE_STOP;
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Settings"))
		{
			ImGui::MenuItem("Options", NULL, &show_configWindow);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			ImGui::MenuItem("About", NULL, &show_aboutWindow);
			ImGui::EndMenu();
		}
	}
	ImGui::EndMainMenuBar();

	if (show_configWindow) ShowConfigWindow(&show_configWindow);
	if (show_aboutWindow) ShowAboutWindow(&show_aboutWindow);

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

void ModuleGuiManager::ShowConfigWindow(bool* open)
{
	ImGui::Begin("Configuration", open);
	ImGui::Spacing();

	if (ImGui::CollapsingHeader("Application"))
	{
		static char appName[120];
		strcpy_s(appName, 120, App->GetAppName());
		if (ImGui::InputText("App Name", appName, 120, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			App->SetAppName(appName);

		static char orgName[120];
		strcpy_s(orgName, 120, App->GetOrgName());
		if (ImGui::InputText("Organization", orgName, 120, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			App->SetOrgName(orgName);

		int maxFps = App->GetFramerateLimit();
		if (ImGui::SliderInt("Max FPS", &maxFps, 1, 120))
			App->SetFramerateLimit(maxFps);

		ImGui::Text("Limit Framerate:");
		ImGui::SameLine();
		ImGui::TextColored(IMGUI_YELLOW, "%i", App->GetFramerateLimit());

		char title[25];
		sprintf_s(title, 25, "Framerate %.1f", fpsLog[fpsLog.size() - 1]);
		ImGui::PlotHistogram("##framerate", &fpsLog[0], fpsLog.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
		
		sprintf_s(title, 25, "Milliseconds %0.1f", frametimeLog[frametimeLog.size() - 1]);
		ImGui::PlotHistogram("##milliseconds", &frametimeLog[0], frametimeLog.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
	}
	if (ImGui::CollapsingHeader("Window"))
	{

	}
	if (ImGui::CollapsingHeader("Hardware"))
	{

	}

	ImGui::End();
}

void ModuleGuiManager::UpdateFrameInfo()
{
	static uint count = 0;

	if (count == 100)
	{
		for (uint i = 0; i < 100 - 1; ++i)
		{
			fpsLog[i] = fpsLog[i + 1];
			frametimeLog[i] = frametimeLog[i + 1];
		}
	}
	else
		++count;

	fpsLog[count - 1] = App->GetFramerate();
	frametimeLog[count - 1] = App->GetFrametime();
}

void ModuleGuiManager::ShowAboutWindow(bool* open)
{
	ImGui::Begin("About", open);

	ImGui::Text("Kyber Engine v1.0");
	ImGui::Text("Developed by Theran1 & yeraytm");
	ImGui::Text("2021 MIT License");

	ImGui::End();
}