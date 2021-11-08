#pragma once
#include "Module.h"
#include "Globals.h"

class Mesh;
class Primitive;
class Plane;

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void ApplyTexture(const char* path);

	void ClearSceneMeshes();
	void ClearSceneTextures();

public:
	std::vector<Mesh*> meshList;

private:
	Plane* scenePlane;
};