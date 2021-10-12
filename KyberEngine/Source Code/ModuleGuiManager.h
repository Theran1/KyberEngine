#ifndef __ModuleGuiManager_H__
#define __ModuleGuiManager_H__

#include "Module.h"
#include "Globals.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#define FPS_LOG_SIZE 100

class Application;

class ModuleGuiManager : public Module
{
public:
	ModuleGuiManager(Application* app, bool start_enabled = true);
	~ModuleGuiManager();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void UpdateFrameInfo();

public:

private:
	void ShowConfigWindow(bool* open);
	void ShowAboutWindow(bool* open);

private:
	bool show_configWindow;
	bool show_aboutWindow;

	std::vector<float> fpsLog;
	std::vector<float> frametimeLog;
};

#endif // __ModuleGuiManager_H__