#pragma once
#include "Module.h"
#include "Globals.h"

class Mesh;

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

public:
	//std::vector<Mesh*> meshList;
};