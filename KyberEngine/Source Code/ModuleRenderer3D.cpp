#include "ModuleRenderer3D.h"
#include "Application.h"

#include "Primitive.h"

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	wireframe = false;

	depthTest = true;
	cullFace = true;
	lighting = true;
	colorMaterial = true;
	texture2D = true;

	usingCheckerTexture = false;
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;

	LOG("OpenGL version supported %s", glGetString(GL_VERSION));

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	// Create context
	context = SDL_GL_CreateContext(App->window->window);
	if (context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	// Initialize Glew
	if (ret)
	{
		GLenum error = glewInit();
		if (error != GLEW_OK)
		{
			LOG("Glew Init failed, something is seriously wrong! Error: %s", glewGetErrorString(error));
			ret = false;
		}
		else
		{
			LOG("Using Glew: %s", glewGetString(GLEW_VERSION));
			LOG("Vendor: %s", glGetString(GL_VENDOR));
			LOG("Renderer: %s", glGetString(GL_RENDERER));
			LOG("OpenGL version supported: %s", glGetString(GL_VERSION));
			LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
		}
	}

	if (ret == true)
	{
		// Use Vsync
		if (VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		// Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		// Check for errors
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		// Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// Check for errors
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);

		// Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		GLfloat LightModelAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);

		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();

		GLfloat MaterialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

		lights[0].Active(true);

		depthTest ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
		cullFace ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
		lighting ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
		colorMaterial ? glEnable(GL_COLOR_MATERIAL) : glDisable(GL_COLOR_MATERIAL);
		texture2D ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);

		glShadeModel(GL_SMOOTH);
	}

	// Projection matrix for
	OnResize(App->window->GetWindowWidth(), App->window->GetWindowHeight());

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for (uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return update_status::UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	SDL_GL_SwapWindow(App->window->window);

	return update_status::UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}

void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	App->window->SetWindowWidth(width);
	App->window->SetWindowHeight(height);
}

bool ModuleRenderer3D::GetWireframe()
{
	return wireframe;
}

void ModuleRenderer3D::ToggleWireframe()
{
	wireframe = !wireframe;
}

bool ModuleRenderer3D::GetDepthTest()
{
	return depthTest;
}

void ModuleRenderer3D::ToggleDepthTest()
{
	depthTest = !depthTest;
	depthTest ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
}

bool ModuleRenderer3D::GetCullFace()
{
	return cullFace;
}

void ModuleRenderer3D::ToggleCullFace()
{
	cullFace = !cullFace;
	cullFace ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
}

bool ModuleRenderer3D::GetLighting()
{
	return lighting;
}

void ModuleRenderer3D::ToggleLighting()
{
	lighting = !lighting;
	lighting ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
}

bool ModuleRenderer3D::GetColorMaterial()
{
	return colorMaterial;
}

void ModuleRenderer3D::ToggleColorMaterial()
{
	colorMaterial = !colorMaterial;
	colorMaterial ? glEnable(GL_COLOR_MATERIAL) : glDisable(GL_COLOR_MATERIAL);
}

bool ModuleRenderer3D::GetTexture2D()
{
	return texture2D;
}

void ModuleRenderer3D::ToggleTexture2D()
{
	texture2D = !texture2D;
	texture2D ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);
}

bool ModuleRenderer3D::GetUsingCheckerTexture() const
{
	return usingCheckerTexture;
}

void ModuleRenderer3D::ToggleCheckerTexture()
{
	usingCheckerTexture = !usingCheckerTexture;
}