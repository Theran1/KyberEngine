#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	// Window Getters
	float	GetWindowBrightness() const;
	uint	GetWindowWidth() const;
	uint	GetWindowHeight() const;
	void	GetScreenRes(uint& w, uint& h) const;
	uint	GetRefreshRate() const;
	bool	GetFullscreen() const;
	bool	GetResizable() const;
	bool	GetBorderless() const;
	bool	GetFullscreenDesktop() const;

	// Window Setters
	void	SetWindowTitle(const char* title);
	void	SetWindowBrightness(float b);
	void	SetWindowWidth(uint w);
	void	SetWindowHeight(uint h);
	void	SetFullscreen(bool f);
	void	SetResizable(bool r);
	void	SetBorderless(bool b);
	void	SetFullscreenDesktop(bool fd);

public:
	// The window we'll be rendering to
	SDL_Window* window;

	// The surface contained by the window
	SDL_Surface* screenSurface;

private:
	uint screenWidth;
	uint screenHeight;
	bool fullscreen;
	bool resizable;
	bool borderless;
	bool fullscreenDesktop;
};

#endif // __ModuleWindow_H__