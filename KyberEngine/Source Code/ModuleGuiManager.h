#ifndef __ModuleGuiManager_H__
#define __ModuleGuiManager_H__

#include "Module.h"
#include "Globals.h"
#include "Tab.h"
#include "TabAbout.h"
#include "TabConfig.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

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

	void AddTab(Tab* tab);

public:
	TabAbout* tabAbout;
	TabConfig* tabConfig;

private:

private:

	std::vector<Tab*> listTabs;

};

#endif // __ModuleGuiManager_H__