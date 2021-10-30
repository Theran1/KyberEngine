#pragma once
#include "Module.h"
#include "Globals.h"

class Mesh;

class ModuleAssImp : public Module
{
public:
	ModuleAssImp(Application* app, bool start_enabled = true);
	~ModuleAssImp();

	bool Init();
	bool Start();
	bool CleanUp();

	void LoadMesh(const char* filePath);

public:
	std::vector<Mesh*> meshList;
};