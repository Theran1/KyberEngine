#include "ModuleScene.h"
#include "Globals.h"
#include "Application.h"

#include "Primitive.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	scenePlane = nullptr;
}

ModuleScene::~ModuleScene()
{}

// Load assets
bool ModuleScene::Start()
{
	LOG("Loading Intro assets");

	bool ret = true;

	App->camera->Move(vec3(1.0f, 5.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	scenePlane = new Plane(0, 1, 0, 0);
	scenePlane->axis = true;

	App->assetsImporter->LoadMesh("Assets/Meshes/baker_house.fbx");

	if (!meshList.empty())
	{
		for (std::vector<Mesh*>::iterator it = meshList.begin(); it != meshList.end(); ++it)
			(*it)->textureID = App->assetsImporter->LoadTexture("Assets/Textures/baker_house.png");
	}

	return ret;
}

// Update: draw background
update_status ModuleScene::Update(float dt)
{
	return update_status::UPDATE_CONTINUE;
}

update_status ModuleScene::PostUpdate(float dt)
{
	scenePlane->Render();

	if (!meshList.empty())
	{
		for (std::vector<Mesh*>::iterator it = meshList.begin(); it != meshList.end(); ++it)
			(*it)->Render(App->renderer3D->GetUsingCheckerTexture());
	}

	return update_status::UPDATE_CONTINUE;
}

// Unload assets
bool ModuleScene::CleanUp()
{
	LOG("Unloading Intro scene");

	RELEASE(scenePlane);

	return true;
}

void ModuleScene::ApplyTexture(const char* path)
{
	if (!meshList.empty())
	{
		for (std::vector<Mesh*>::iterator it = meshList.begin(); it != meshList.end(); ++it)
			(*it)->textureID = App->assetsImporter->LoadTexture(path);
	}
}

void ModuleScene::ClearSceneMeshes()
{
	if (!meshList.empty())
	{
		for (std::vector<Mesh*>::iterator it = meshList.begin(); it != meshList.end(); ++it)
		{
			(*it)->~Mesh();
		}
	}
	meshList.clear();
}

void ModuleScene::ClearSceneTextures()
{
	if (!meshList.empty())
	{
		for (std::vector<Mesh*>::iterator it = meshList.begin(); it != meshList.end(); ++it)
			(*it)->textureID = 0;
	}
}