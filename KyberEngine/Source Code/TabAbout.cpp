#include "TabAbout.h"
#include "Application.h"

TabAbout::TabAbout(Application* app) : Tab(app)
{}

TabAbout::~TabAbout()
{}

update_status TabAbout::Update()
{
	ImGui::Begin("About", &active);

	ImGui::Text("Kyber Engine v1.0");
	ImGui::Text("Developed by Theran1 & yeraytm");
	ImGui::Text("2021 MIT License");

	ImGui::End();

	return update_status::UPDATE_CONTINUE;
}