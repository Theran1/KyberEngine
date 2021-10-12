#pragma once

#include "Globals.h"
#include "Timer.h"
#include "PerfTimer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
//#include "ModuleAudio.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleGuiManager.h"

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	// GETTERS & SETTERS
	const char* GetAppName() const;
	void SetAppName(const char* name);

	const char* GetOrgName() const;
	void SetOrgName(const char* name);

	uint GetFramerateLimit() const;
	void SetFramerateLimit(uint maxFramerate);

	float GetFramerate() const;
	float GetFrametime() const;

public:

	ModuleWindow* window;
	ModuleInput* input;
	//ModuleAudio* audio;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleGuiManager* guiManager;

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

private:

	// Timing measures
	uint frameCount;
	float frametimeMs;
	float dt;
	uint fpsCap;

	float avgFramerate;

	PerfTimer perfTimer;
	Timer frametime;
	Timer lastSecFramesTime;

	std::list<Module*> listModules;

	std::string appName;
	std::string orgName;
};