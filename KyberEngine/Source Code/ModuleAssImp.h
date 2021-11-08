#pragma once
#include "Module.h"
#include "Globals.h"

class Mesh;
class aiMesh;

class ModuleAssImp : public Module
{
public:
	ModuleAssImp(Application* app, bool start_enabled = true);
	~ModuleAssImp();

	bool Init();
	bool Start();
	bool CleanUp();

	void LoadMesh(const char* path);
	uint LoadTexture(const char* path);
};