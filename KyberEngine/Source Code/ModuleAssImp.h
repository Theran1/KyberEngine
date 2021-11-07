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

	//void LoadModel(const char* fileName);
	void LoadMesh(const char* meshFilename);
	uint LoadTexture(const char* path);

public:
};