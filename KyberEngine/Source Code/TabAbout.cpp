#include "TabAbout.h"
#include "Application.h"

#include "version.h"

TabAbout::TabAbout(Application* app) : Tab(app)
{}

TabAbout::~TabAbout()
{}

update_status TabAbout::Update()
{
	ImGui::Begin("About", &active);

	ImGui::SetWindowFontScale(1.1f);
	ImGui::TextColored(IMGUI_GREEN, "Kyber Engine v0.1");
	ImGui::SetWindowFontScale(1);
	ImGui::Spacing();

	ImGui::Bullet();
	if (ImGui::Button("Check our Repository"))
		ShellExecuteA(NULL, "open", "https://github.com/Theran1/KyberEngine", NULL, NULL, SW_SHOWNORMAL);
	
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Text("Developed by:");
	ImGui::Spacing();

	IMGUI_BULLET(IMGUI_BLUE, "Abraham Diaz:", "");
	ImGui::SameLine();
	if (ImGui::Button("Theran1"))
		ShellExecuteA(NULL, "open", "https://github.com/Theran1", NULL, NULL, SW_SHOWNORMAL);
	ImGui::Spacing();

	IMGUI_BULLET(IMGUI_BLUE, "Yeray Tarifa:", "");
	ImGui::SameLine();
	if (ImGui::Button("yeraytm"))
		ShellExecuteA(NULL, "open", "https://github.com/yeraytm", NULL, NULL, SW_SHOWNORMAL);

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Text("3rd Party Libraries used:");
	ImGui::Spacing();

	SDL_version sdlVersion;
	SDL_GetVersion(&sdlVersion);
	IMGUI_BULLET(IMGUI_RED, "SDL", "%i.%i.%i", sdlVersion.major, sdlVersion.minor, sdlVersion.patch);
	ImGui::Spacing();

	IMGUI_BULLET(IMGUI_RED, "OpenGL", "%s", glGetString(GL_VERSION));
	ImGui::Spacing();

	IMGUI_BULLET(IMGUI_RED, "Glew", "%s", glewGetString(GLEW_VERSION));
	ImGui::Spacing();

	IMGUI_BULLET(IMGUI_RED, "ImGui", "%s", ImGui::GetVersion());
	ImGui::Spacing();

	IMGUI_BULLET(IMGUI_RED, "Assimp", "%i.%i.%i", aiGetVersionMajor(), aiGetVersionMinor(), aiGetVersionRevision());
	ImGui::Spacing();

	IMGUI_BULLET(IMGUI_RED, "stb_image", "2.27");

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::TextColored(IMGUI_BLUE, "MIT License");
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Text("Copyright (c) 2021 Theran1 yeraytm");
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::TextWrapped("Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and /or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: ");
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::TextWrapped("The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.");
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::TextWrapped("THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.");

	ImGui::End();

	return update_status::UPDATE_CONTINUE;
}