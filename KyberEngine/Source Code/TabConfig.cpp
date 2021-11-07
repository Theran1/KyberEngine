#include "TabConfig.h"
#include "Application.h"

TabConfig::TabConfig(Application* app) : Tab(app), framerateLog(FPS_LOG_SIZE), frametimeLog(FPS_LOG_SIZE)
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
		sprintf_s(title, 25, "Framerate: %.1f FPS", framerateLog[framerateLog.size() - 1]);
		ImGui::PlotHistogram("##framerate", &framerateLog[0], framerateLog.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));

		sprintf_s(title, 25, "Frametime: %0.1f ms", frametimeLog[frametimeLog.size() - 1]);
		ImGui::PlotHistogram("##frametime", &frametimeLog[0], frametimeLog.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
	}
	if (ImGui::CollapsingHeader("Window"))
	{
		float brightness = app->window->GetWindowBrightness();
		if (ImGui::SliderFloat("Window Brightness", &brightness, 0.3f, 1.0f))
			app->window->SetWindowBrightness(brightness);

		uint windowW = app->window->GetWindowWidth();
		uint windowH = app->window->GetWindowHeight();
		uint screenResW, screenResH;
		app->window->GetScreenRes(screenResW, screenResH);

		if (ImGui::SliderInt("Width", (int*)&windowW, 1024, screenResW))
		{
			app->window->SetWindowWidth(windowW);
			app->renderer3D->OnResize(windowW, windowH);
		}

		if (ImGui::SliderInt("Height", (int*)&windowH, 768, screenResH))
		{
			app->window->SetWindowHeight(windowH);
			app->renderer3D->OnResize(windowW,windowH);
		}

		if (ImGui::Button("Reset Window's Size"))
		{
			app->window->SetWindowWidth(1600);
			app->window->SetWindowHeight(900);
			app->renderer3D->OnResize(1600, 900);
		}

		ImGui::Text("Refresh rate:");
		ImGui::SameLine();
		ImGui::TextColored(IMGUI_GREEN, "%u Hz", app->window->GetRefreshRate());

		ImGui::Separator();

		bool fullscreen = app->window->GetFullscreen();
		if (ImGui::Checkbox("Fullscreen", &fullscreen))
			app->window->SetFullscreen(fullscreen);

		ImGui::SameLine();
		bool resizable = app->window->GetResizable();
		if (ImGui::Checkbox("Resizable", &resizable))
			app->window->SetResizable(resizable);

		bool borderless = app->window->GetBorderless();
		if (ImGui::Checkbox("Borderless", &borderless))
			app->window->SetBorderless(borderless);

		ImGui::SameLine();
		bool fullscreenDesktop = app->window->GetFullscreenDesktop();
		if (ImGui::Checkbox("Fullscreen Desktop", &fullscreenDesktop))
			app->window->SetFullscreenDesktop(fullscreenDesktop);
	}
	if (ImGui::CollapsingHeader("Hardware"))
	{
		SDL_version sdlVersion;
		SDL_GetVersion(&sdlVersion);
		IMGUI_COLOR_PARAM(IMGUI_GREEN, "SDL Version:", "%i.%i.%i", sdlVersion.major, sdlVersion.minor, sdlVersion.patch);

		ImGui::Separator();

		IMGUI_COLOR_PARAM(IMGUI_GREEN, "CPUs:", "%d (Cache: %u KB)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());
		IMGUI_COLOR_PARAM(IMGUI_GREEN, "System RAM:", "%.1f GB", float(SDL_GetSystemRAM() / 1024.0f));
		IMGUI_COLOR_PARAM(IMGUI_GREEN, "Caps: ", "%s%s%s%s%s%s%s%s%s%s%s",
			SDL_HasAVX() ? "AVX," : "",
			SDL_HasAVX2() ? "AVX2," : "",
			SDL_HasMMX() ? "MMX," : "",
			SDL_HasRDTSC() ? "RDTSC," : "",
			SDL_HasSSE() ? "SSE," : "",
			SDL_HasSSE2() ? "SSE2," : "",
			SDL_HasSSE3() ? "SSE3," : "",
			SDL_HasSSE41() ? "SSE4.1," : "",
			SDL_HasSSE42() ? "SSE4.2," : "",
			SDL_Has3DNow() ? "3DNow!," : "",
			SDL_HasAltiVec() ? "AltiVec" : "");

		ImGui::Separator();

		IMGUI_COLOR_PARAM(IMGUI_GREEN, "GPU:", "%s", glGetString(GL_RENDERER));
		IMGUI_COLOR_PARAM(IMGUI_GREEN, "Brand:", "%s", glGetString(GL_VENDOR));

		// VRAM in KB
		GLint sizeVRAM = 0;
		GLint availableVRAM = 0;

		glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &sizeVRAM);
		glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &availableVRAM);
		// VRAM in KB to MB
		sizeVRAM /= 1000;
		availableVRAM /= 1000;
		IMGUI_COLOR_PARAM(IMGUI_GREEN, "VRAM Size:", "%d MB", sizeVRAM);
		IMGUI_COLOR_PARAM(IMGUI_GREEN, "VRAM Usage:", "%d MB", sizeVRAM - availableVRAM);
		IMGUI_COLOR_PARAM(IMGUI_GREEN, "VRAM Available:", "%d MB", availableVRAM);
	}
	if (ImGui::CollapsingHeader("Renderer"))
	{
		bool depthTest = app->renderer3D->GetDepthTest();
		if (ImGui::Checkbox("Depth Test", &depthTest))
			app->renderer3D->ToggleDepthTest();
		ImGui::SameLine();

		bool cullFace = app->renderer3D->GetCullFace();
		if (ImGui::Checkbox("Cull Face", &cullFace))
			app->renderer3D->ToggleCullFace();
		
		bool lighting = app->renderer3D->GetLighting();
		if (ImGui::Checkbox("Lighting  ", &lighting))
			app->renderer3D->ToggleLighting();
		ImGui::SameLine();

		bool colorMaterial = app->renderer3D->GetColorMaterial();
		if (ImGui::Checkbox("Color Material", &colorMaterial))
			app->renderer3D->ToggleColorMaterial();

		bool texture2D = app->renderer3D->GetTexture2D();
		if (ImGui::Checkbox("Texture 2D", &texture2D))
			app->renderer3D->ToggleTexture2D();
		ImGui::SameLine();

		bool lights = app->renderer3D->lights[0].GetLight();
		if (ImGui::Checkbox("Directional Light", &lights))
			app->renderer3D->lights[0].Active(lights);
	}
	if (ImGui::CollapsingHeader("Textures"))
	{
		bool checkerTexture = app->renderer3D->GetUsingCheckerTexture();
		if (ImGui::Checkbox("Checker Texture", &checkerTexture))
			app->renderer3D->ToggleCheckerTexture();
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
			framerateLog[i] = framerateLog[i + 1];
			frametimeLog[i] = frametimeLog[i + 1];
		}
	}
	else
		++count;

	framerateLog[count - 1] = framerate;
	frametimeLog[count - 1] = frametime;
}
