#pragma once
#include "Module.h"
#include "Globals.h"

class ModuleImporter : public Module
{
public:
	ModuleImporter(Application* app, bool start_enabled = true);
	~ModuleImporter();

	bool Start();
	bool CleanUp();

};