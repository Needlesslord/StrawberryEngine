#ifndef __ModuleUI_H__
#define __ModuleUI_H__

#include "Module.h"
#include "Libs/SDL/include/SDL.h"

#include <GL/glew.h>

#include "Libs/ImGui/imgui.h"
#include "Libs/ImGui/imgui_impl_sdl.h"
#include "Libs/ImGui/imgui_impl_opengl3.h"

class Application;

class ModuleUI : public Module
{
public:

	ModuleUI(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleUI();

	bool Init();

	bool Start();

	update_status PreUpdate(float dt);

	update_status Update(float dt);

	update_status PostUpdate(float dt);

	bool CleanUp();

	void Draw();

	void AddConsoleOutput(const char* text);

	void UnselectAll();

	void CreateHierarchy(GameObject* go);

public:

	bool done = false;

	// Windows shown bools
	bool isDemoShown = false;

	bool isConfigShown = false;
	bool isConfigInit = false;

	bool isAboutShown = false;
	bool isAboutInit = false;

	bool isHierarchyShown = false;
	bool isHierarchyInit = false;

	bool isInspectorShown = false;
	bool isInspectorInit = false;

	bool isConsoleShown = true;
	bool isConsoleInit = false;

	bool isReparentingShown = false;

	// Config data
	char* name = nullptr;
	char* organization = nullptr;

			// Temporary vars
	bool isActive = false;
	float brightness = 1.0f;
	bool isWinFullscreen;
	bool isWinResizable;
	bool isWinBorderless;
	bool isWinFullscreenDesktop;

	bool isDrawEnabled = true;
	bool isDepthTestEnabled = true;
	bool isLightingEnabled = true;
	bool isBackFaceCullEnabled = true;
	bool isTexturesEnabled = true;
	bool isWireframeEnabled = false;

	bool isVertexNormalsEnabled = false;
	bool isFaceNormalsEnabled = false;

	std::list<const char*> pendingOutputs;

};

#endif // __ModuleUI_H__