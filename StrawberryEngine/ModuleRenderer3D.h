#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"

#define MAX_LIGHTS 8

class GameObject;
class Texture;
class ComponentCamera;

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

	void DrawCubeDirect();
	void DrawCubeArray();
	void DrawCubeIndices();

	void DrawCameraFrustum(ComponentCamera* camera);

	void GenerateBuffers();
	void Draw(GameObject* go);

	Texture* CreateCheckersTexture();

	void ToggleDepthTest(const bool switchTo);
	void ToggleLighting(const bool switchTo);
	void ToggleBackFaceCull(const bool switchTo);
	void ToggleTextures(const bool switchTo);
	void ToggleWireframe(const bool switchTo);

public:

	bool isToggleWireframe = false;

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	Color background;

	uint my_id = 0;
	uint myId = 0;
	uint myIndeces = 0;

	uint frameBuffer = 0;
	uint texColorBuffer = 0;
	uint rboDepthStencil = 0;

	bool swap = true;
};