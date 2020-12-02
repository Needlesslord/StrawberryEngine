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

	void ShowMenuBar();
	void ShowConfig();
	void ShowHierarchy();
	void ShowInspector();
	void ShowConsole();
	void ShowAbout();
	void ShowAssets();

	void CreateHierarchy(GameObject* go);

public:

	bool done = false;

	// Windows shown bools
	bool isDemoActive = false;

	bool isConfigActive = false;
	bool isConfigInit = false;

	bool isAboutActive = false;
	bool isAboutInit = false;

	bool isHierarchyActive = false;
	bool isHierarchyInit = false;

	bool isInspectorActive = false;
	bool isInspectorInit = false;

	bool isConsoleActive = true;
	bool isConsoleInit = false;

	bool isAssetsActive = true;
	bool isAssetsInit = false;

	bool isReparentingActive = false;
	bool isChangeTexActive = false;
	bool isChangeMeshActive = false;
	bool isAddTexActive = false;
	bool isAddMeshActive = false;

	// Config data
	char* name = nullptr;
	char* organization = nullptr;

	// Temporary vars
	float brightness = 1.0f;
	bool isWinFullscreen; // Started on Start()
	bool isWinResizable;
	bool isWinBorderless;
	bool isWinFullscreenDesktop;

	bool isDrawEnabled = true;
	bool isDepthTestEnabled = true;
	bool isLightingEnabled = true;
	bool isBackFaceCullEnabled = true;
	bool isTexturesEnabled = true;
	bool isWireframeEnabled = false;

	std::list<const char*> pendingOutputs;

};

#endif // __ModuleUI_H__