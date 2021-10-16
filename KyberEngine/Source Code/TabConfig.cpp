#include "TabConfig.h"
#include "Application.h"

TabConfig::TabConfig(Application* app) : Tab(app), fpsLog(FPS_LOG_SIZE), frametimeLog(FPS_LOG_SIZE)
{}

TabConfig::~TabConfig()
{}

update_status TabConfig::Update()
{
	ImGui::Begin("Configuration", &active);
	ImGui::Spacing();

	if (ImGui::CollapsingHeader("Application"))
	{
		static char appName[120];
		strcpy_s(appName, 120, app->GetAppName());
		if (ImGui::InputText("App Name", appName, 120, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			app->SetAppName(appName);

		static char orgName[120];
		strcpy_s(orgName, 120, app->GetOrgName());
		if (ImGui::InputText("Organization", orgName, 120, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			app->SetOrgName(orgName);

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		int maxFps = app->GetFramerateLimit();
		if (ImGui::SliderInt("FPS Cap", &maxFps, 1, 120))
			app->SetFramerateLimit(maxFps);

		ImGui::Text("Framerate cap:");
		ImGui::SameLine();
		ImGui::TextColored(IMGUI_YELLOW, "%i", app->GetFramerateLimit());

		char title[25];
		sprintf_s(title, 25, "Framerate: %.1f FPS", fpsLog[fpsLog.size() - 1]);
		ImGui::PlotHistogram("##framerate", &fpsLog[0], fpsLog.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));

		sprintf_s(title, 25, "Frametime: %0.1f ms", frametimeLog[frametimeLog.size() - 1]);
		ImGui::PlotHistogram("##frametime", &frametimeLog[0], frametimeLog.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
	}
	if (ImGui::CollapsingHeader("Window"))
	{
		float brightness = app->window->GetWindowBrightness();
		if (ImGui::SliderFloat("Window Brightness", &brightness, 0.1f, 1.0f))
			app->window->SetWindowBrightness(brightness);


		uint windowW = app->window->GetWindowWidth();
		uint windowH = app->window->GetWindowHeight();
		uint screenResW, screenResH;
		app->window->GetScreenRes(screenResW, screenResH);

		if (ImGui::SliderInt("Width", (int*)&windowW, 1024, screenResW))
			app->window->SetWindowWidth(windowW);

		if (ImGui::SliderInt("Height", (int*)&windowH, 768, screenResH))
			app->window->SetWindowHeight(windowH);

		if (ImGui::Button("Reset Window's Size"))
		{
			app->window->SetWindowWidth(1024);
			app->window->SetWindowHeight(768);
		}

		ImGui::Text("Refresh rate:");
		ImGui::SameLine();
		ImGui::TextColored(IMGUI_GREEN, "%u", app->window->GetRefreshRate());

		ImGui::Separator();

		bool fullscreen = app->window->GetFullscreen();
		if (ImGui::Checkbox("Fullscreen", &fullscreen))
			app->window->SetFullscreen(fullscreen);

		bool resizable = app->window->GetResizable();
		if (ImGui::Checkbox("Resizable", &resizable))
			app->window->SetResizable(resizable);

		bool borderless = app->window->GetBorderless();
		if (ImGui::Checkbox("Borderless", &borderless))
			app->window->SetBorderless(borderless);

		bool fullscreenDesktop = app->window->GetFullscreenDesktop();
		if (ImGui::Checkbox("Fullscreen Desktop", &fullscreenDesktop))
			app->window->SetFullscreenDesktop(fullscreenDesktop);
	}
	if (ImGui::CollapsingHeader("Hardware"))
	{
		SDL_version sdlVersion;
		SDL_GetVersion(&sdlVersion);
		IMGUI_PRINT("SDL Version:", "%i.%i.%i", sdlVersion.major, sdlVersion.minor, sdlVersion.patch);

		ImGui::Separator();

		IMGUI_PRINT("CPUs:", "%d (Cache: %u KB)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());
		IMGUI_PRINT("System RAM:", "%.1f GB", float(SDL_GetSystemRAM() / 1024.0f));
	}
	ImGui::End();

	return update_status::UPDATE_CONTINUE;
}

void TabConfig::UpdateFrameInfo(float framerate, float frametime)
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

	fpsLog[count - 1] = framerate;
	frametimeLog[count - 1] = frametime;
}
