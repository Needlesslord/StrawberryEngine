#pragma once

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleUI.h"
#include "ModuleAssetImporter.h"
#include "ModuleFileSystem.h"

#include <iostream>
#include <list>
#include <vector>

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleUI* ui;
	ModuleAssetImporter* assetImporter;
	ModuleFileSystem* fileSystem;

private:

	Timer	ms_timer;
	float	dt;
	std::list<Module*> list_modules;

	std::string title;
	std::string organization;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	void RequestBrowser(const char* URL);

	const char* GetTitleName() const;
	const char* GetOrganizationName() const;

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

public:

	int maxFps = 60;
	std::vector<float> fps;
	std::vector<float> ms;




};