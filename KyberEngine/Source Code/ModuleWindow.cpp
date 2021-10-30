#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	window = NULL;
	screenSurface = NULL;

	screenWidth = 1024;
	screenHeight = 768;
	fullscreen = false;
	resizable = true;
	borderless = false;
	fullscreenDesktop = false;
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

		if (fullscreenDesktop == true)
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
			screenSurface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	// Destroy window
	if (window != nullptr)
		SDL_DestroyWindow(window);

	// Quit SDL subsystems
	SDL_Quit();

	return true;
}

// Window Getters ---
float ModuleWindow::GetWindowBrightness() const
{
	return SDL_GetWindowBrightness(window);
}

uint ModuleWindow::GetWindowWidth() const
{
	return screenWidth;
}

uint ModuleWindow::GetWindowHeight() const
{
	return screenHeight;
}

void ModuleWindow::GetScreenRes(uint& width, uint& height) const
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

bool ModuleWindow::GetResizable() const
{
	return resizable;
}

bool ModuleWindow::GetBorderless() const
{
	return borderless;
}

bool ModuleWindow::GetFullscreenDesktop() const
{
	return fullscreenDesktop;
}

// Window Setters ---
void ModuleWindow::SetWindowTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

void ModuleWindow::SetWindowBrightness(float brightness)
{
	if (SDL_SetWindowBrightness(window, brightness) != 0)
		LOG("Could not change window brightness: %s\n", SDL_GetError());
}

void ModuleWindow::SetWindowWidth(uint width)
{
	screenWidth = width;
	SDL_SetWindowSize(window, screenWidth, GetWindowHeight());
}

void ModuleWindow::SetWindowHeight(uint height)
{
	screenHeight = height;
	SDL_SetWindowSize(window, GetWindowWidth(), screenHeight);
}

void ModuleWindow::SetFullscreen(bool fullscreen)
{
	if (fullscreen != this->fullscreen)
	{
		this->fullscreen = fullscreen;

		if (this->fullscreen == true)
		{
			if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN) != 0)
				LOG("Could not switch to fullscreen: %s\n", SDL_GetError());

			fullscreenDesktop = false;
		}
		else
		{
			if (SDL_SetWindowFullscreen(window, 0) != 0)
				LOG("Could not switch to windowed: %s\n", SDL_GetError());
		}
	}
}

void ModuleWindow::SetResizable(bool resizable)
{
	// cannot be changed while the program is running, but we can save the change
	this->resizable = resizable;
}

void ModuleWindow::SetBorderless(bool borderless)
{
	if (borderless != this->borderless && fullscreen == false && fullscreenDesktop == false)
	{
		this->borderless = borderless;
		SDL_SetWindowBordered(window, (SDL_bool)!this->borderless);
	}
}

void ModuleWindow::SetFullscreenDesktop(bool fullscreenDesktop)
{
	if (fullscreenDesktop != this->fullscreenDesktop)
	{
		this->fullscreenDesktop = fullscreenDesktop;

		if (this->fullscreenDesktop == true)
		{
			if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0)
				LOG("Could not switch to fullscreen desktop: %s\n", SDL_GetError());

			fullscreen = false;
		}
		else
		{
			if (SDL_SetWindowFullscreen(window, 0) != 0)
				LOG("Could not switch to windowed: %s\n", SDL_GetError());
		}
	}
}