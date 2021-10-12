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
	int		GetWindowWidth() const;
	int		GetWindowHeight() const;
	void	GetScreenRes(int& w, int& h) const;
	uint	GetRefreshRate() const;
	bool	GetFullscreen() const;
	bool	GetResizable() const;
	bool	GetBorderless() const;
	bool	GetFullDesktop() const;

	// Window Setters
	void	SetWindowTitle(const char* title);
	void	SetWindowBrightness(float b);
	void	SetWindowWidth(int w);
	void	SetWindowHeight(int h);
	void	SetFullscreen(bool f);
	void	SetResizable(bool r);
	void	SetBorderless(bool b);
	void	SetFullscreenDesktop(bool fd);

public:
	// The window we'll be rendering to
	SDL_Window* window;

	// The surface contained by the window
	SDL_Surface* screen_surface;

private:
	int screenWidth;
	int screenHeight;
	bool fullscreen;
	bool resizable;
	bool borderless;
	bool fullDesktop;
};

#endif // __ModuleWindow_H__