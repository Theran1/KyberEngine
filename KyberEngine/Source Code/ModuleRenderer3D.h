#pragma once
#include "RendererGlobals.h"
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"

#define MAX_LIGHTS 8

typedef void* SDL_GLContext;

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

	bool GetWireframe();
	void ToggleWireframe();

	bool GetDepthTest();
	void ToggleDepthTest();

	bool GetCullFace();
	void ToggleCullFace();

	bool GetLighting();
	void ToggleLighting();

	bool GetColorMaterial();
	void ToggleColorMaterial();

	bool GetTexture2D();
	void ToggleTexture2D();

	//bool GetVSync();
	//void SetVSync(bool _vsync);

	bool GetUsingCheckerTexture() const;
	void ToggleCheckerTexture();

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

private:
	bool wireframe = false;

	bool depthTest = true;
	bool cullFace = true;
	bool lighting = true;
	bool colorMaterial = true;
	bool texture2D = true;

	//bool vSync = false;

	bool usingCheckerTexture;
};