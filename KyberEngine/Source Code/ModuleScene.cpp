#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Primitive.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleScene::~ModuleScene()
{}

// Load assets
bool ModuleScene::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));



	//Plane* p = new Plane(0, 1, 0, 0);
	//p->axis = true;
	////App->renderer3D->AddPrimitive(p);

	//c = new Cube(1, 1, 1);
	//c->SetPos(-2, 0, 0);
	//App->renderer3D->AddPrimitive(c);

	//Cylinder* cyl = new Cylinder(1, 3);
	//cyl->SetPos(2, 0, 0);
	//App->renderer3D->AddPrimitive(cyl);


	//pyd = new Pyramid(2, 2, 2);
	//pyd->SetPos(2, 0, -3);
	//App->renderer3D->AddPrimitive(pyd);

	//Sphere* s = new Sphere();
	//s->SetPos(0, 0, -3);
	//App->renderer3D->AddPrimitive(s);

	//std::vector<CustomMesh*>::iterator w = customMeshes.begin();
	//(*w)->SetPos(0, 0, 1);
	////(*w)->SetRotation(45.0f, (1.0f, 0.0f, 0.0f)); TODO
	//(*w)->Scale(0.05, 0.05, 0.05);
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


	return UPDATE_CONTINUE;
}


update_status ModuleScene::PostUpdate()
{
	App->renderer3D->Render();
	return UPDATE_CONTINUE;
}