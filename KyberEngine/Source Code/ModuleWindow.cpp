#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	window = NULL;
	screen_surface = NULL;

	screenWidth = 1024;
	screenHeight = 768;
	fullscreen = false;
	resizable = true;
	borderless = false;
	fullDesktop = false;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		// Create window
		int width = screenWidth * SCREEN_SIZE;
		int height = screenHeight * SCREEN_SIZE;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		// Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if (fullscreen == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if (resizable == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if (borderless == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if (fullDesktop == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(App->GetAppName(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if (window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			// Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	// Destroy window
	if (window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	// Quit SDL subsystems
	SDL_Quit();

	return true;
}

void ModuleWindow::SetWindowTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

float ModuleWindow::GetWindowBrightness() const
{
	return SDL_GetWindowBrightness(window);
}

void ModuleWindow::SetWindowBrightness(float brightness)
{
	SDL_SetWindowBrightness(window, brightness);
}

int ModuleWindow::GetWindowWidth() const
{
	return screenWidth;
}

void ModuleWindow::SetWindowWidth(int width)
{
	screenWidth = width;
	SDL_SetWindowSize(window, screenWidth, screenHeight);
}

int ModuleWindow::GetWindowHeight() const
{
	return screenHeight;
}

void ModuleWindow::SetWindowHeight(int height)
{
	screenHeight = height;
	SDL_SetWindowSize(window, screenWidth, screenHeight);
}

void ModuleWindow::GetScreenRes(int& width, int& height) const
{
	SDL_DisplayMode displayMode;
	if (SDL_GetDesktopDisplayMode(0, &displayMode) != 0)
	{
		LOG("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
	}
	else
	{
		width = displayMode.w;
		height = displayMode.h;
	}
}

uint ModuleWindow::GetRefreshRate() const
{
	uint ret = 0;

	SDL_DisplayMode displayMode;
	if (SDL_GetDesktopDisplayMode(0, &displayMode) != 0)
	{
		LOG("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
	}
	else
	{
		ret = displayMode.refresh_rate;
	}

	return ret;
}

bool ModuleWindow::GetFullscreen() const
{
	return fullscreen;
}

void ModuleWindow::SetFullscreen(bool fullscreen)
{
	if (fullscreen != this->fullscreen)
	{
		this->fullscreen = fullscreen;

		if (this->fullscreen == true)
		{
			if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN) != 0)
			{
				LOG("Could not switch to fullscreen: %s\n", SDL_GetError());
			}
			fullDesktop = false;
		}
		else
		{
			if (SDL_SetWindowFullscreen(window, 0) != 0)
			{
				LOG("Could not switch to windowed: %s\n", SDL_GetError());
			}
		}
	}
}

bool ModuleWindow::GetResizable() const
{
	return resizable;
}

void ModuleWindow::SetResizable(bool resizable)
{
	//TODO: save and load this value
	this->resizable = resizable;
}

bool ModuleWindow::GetBorderless() const
{
	return borderless;
}

void ModuleWindow::SetBorderless(bool borderless)
{
	if (borderless != this->borderless && fullscreen == false && fullDesktop == false)
	{
		this->borderless = borderless;
		SDL_SetWindowBordered(window, (SDL_bool)!this->borderless);
	}
}

bool ModuleWindow::GetFullDesktop() const
{
	return fullDesktop;
}

void ModuleWindow::SetFullscreenDesktop(bool fullscreenDesktop)
{
	if (fullscreenDesktop != this->fullDesktop)
	{
		this->fullDesktop = fullscreenDesktop;
		if (this->fullDesktop == true)
		{
			if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0)
			{
				LOG("Could not switch to fullscreen desktop: %s\n", SDL_GetError());
			}
			fullscreen = false;
		}
		else
		{
			if (SDL_SetWindowFullscreen(window, 0) != 0)
			{
				LOG("Could not switch to windowed: %s\n", SDL_GetError());
			}
		}
	}
}