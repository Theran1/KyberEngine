#pragma once
#include "Module.h"
#include "Globals.h"


class Pyramid;
class Cube;
class CustomMesh;

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Start();
	update_status Update(float dt);
	update_status PostUpdate();
	bool CleanUp();

public:
	Cube* c;
	Pyramid* pyd;
	std::vector<CustomMesh*> customMeshes;
	
};