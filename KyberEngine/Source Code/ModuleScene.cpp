#include "ModuleScene.h"
#include "Globals.h"
#include "Application.h"

#include "Primitive.h"
#include "ModuleAssImp.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleScene::~ModuleScene()
{}

// Load assets
bool ModuleScene::Start()
{
	LOG("Loading Intro assets");

	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	return ret;
}

// Load assets
bool ModuleScene::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleScene::Update(float dt)
{
	return update_status::UPDATE_CONTINUE;
}


update_status ModuleScene::PostUpdate(float dt)
{
	for (std::vector<Mesh*>::iterator it = App->assetsImporter->meshList.begin(); it != App->assetsImporter->meshList.end(); ++it)
		(*it)->Render();

	//while (item != App->importer->listMesh.end())
	//{
	//	(*item)->wire = wireframe;
	//	(*item)->drawFaceNormals = faceNormals;
	//	(*item)->drawVertexNormals = vecNormals;
	//	(*item)->Render();
	//	++item;
	//}

	return update_status::UPDATE_CONTINUE;
}