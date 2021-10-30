#include "TabAbout.h"
#include "Application.h"

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

	ImGui::Text("Developed by Theran1 & yeraytm");

	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Text("3rd Party Libraries used:");
	ImGui::BulletText("SDL");
	ImGui::BulletText("OpenGL");
	ImGui::BulletText("Glew");
	ImGui::BulletText("MathGeoLib");

	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Text("2021 MIT License");

	ImGui::End();

	return update_status::UPDATE_CONTINUE;
}