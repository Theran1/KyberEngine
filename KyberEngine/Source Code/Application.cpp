#include "Application.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	//audio = new ModuleAudio(this, true);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	guiManager = new ModuleGuiManager(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	//AddModule(audio);
	AddModule(guiManager);

	// Renderer last!
	AddModule(renderer3D);

	appName = "Kyber Engine";
	orgName = "UPC-CITM";

	frameCount = 0;
	frametimeMs = 0;
	dt = 0.0f;

	avgFramerate = 0.0f;

	fpsCap = 60;

	lastSecFramesTime.Stop();
}

Application::~Application()
{
	for (std::list<Module*>::reverse_iterator it = listModules.rbegin(); it != listModules.rend(); ++it)
	{
		RELEASE(*it);
	}
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	for (std::list<Module*>::iterator it = listModules.begin(); it != listModules.end() && ret; ++it)
	{
		ret = (*it)->Init();
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	
	for (std::list<Module*>::iterator it = listModules.begin(); it != listModules.end() && ret; ++it)
	{
		ret = (*it)->Start();
	}
	
	lastSecFramesTime.Start();

	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = frametime.ReadSec();
	frameCount++;

	frametime.Start();

	perfTimer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	if (lastSecFramesTime.Read() >= 1000)
	{
		avgFramerate = float(frameCount) / lastSecFramesTime.ReadSec();

		lastSecFramesTime.Start(); 
		frameCount = 0;
	}

	frametimeMs = (float)perfTimer.ReadMs();

	float msCap = float(1000.0f / fpsCap);

	if ((msCap > 0) && (msCap > frametimeMs))
	{
		SDL_Delay(msCap - frametimeMs);
		frametimeMs = (float)perfTimer.ReadMs();
		perfTimer.Start();
	}
	else
		perfTimer.Start();

	guiManager->UpdateFrameInfo();
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	for (std::list<Module*>::iterator it = listModules.begin(); it != listModules.end() && ret == UPDATE_CONTINUE; ++it)
	{
		ret = (*it)->PreUpdate(dt);
	}

	for (std::list<Module*>::iterator it = listModules.begin(); it != listModules.end() && ret == UPDATE_CONTINUE; ++it)
	{
		ret = (*it)->Update(dt);
	}

	for (std::list<Module*>::iterator it = listModules.begin(); it != listModules.end() && ret == UPDATE_CONTINUE; ++it)
	{
		ret = (*it)->PostUpdate(dt);
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for (std::list<Module*>::reverse_iterator it = listModules.rbegin(); it != listModules.rend() && ret; ++it)
	{
		ret = (*it)->CleanUp();
	}

	return ret;
}

void Application::AddModule(Module* mod)
{
	listModules.push_back(mod);
}

// PUBLIC METHODS (GETTERS & SETTERS)
const char* Application::GetAppName() const
{
	return appName.c_str();
}

void Application::SetAppName(const char* name)
{
	if (name != nullptr && name != appName)
	{
		appName = name;
		window->SetWindowTitle(name);
	}
}

const char* Application::GetOrgName() const
{
	return orgName.c_str();
}

void Application::SetOrgName(const char* name)
{
	if (name != nullptr && name != orgName)
	{
		orgName = name;
	}
}

uint Application::GetFramerateLimit() const
{
	if (fpsCap > 0)
		return fpsCap;
	else
		return 0;
}

void Application::SetFramerateLimit(uint maxFramerate)
{
	if (maxFramerate > 0)
		fpsCap = maxFramerate;
	else
		fpsCap = 0;
}

float Application::GetFramerate() const
{
	return avgFramerate;
}

float Application::GetFrametime() const
{
	return frametimeMs;
}