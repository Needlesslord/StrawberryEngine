#include "Globals.h"
#include "Application.h"
#include "ModuleImporter.h"

#include "Libs/Assimp/include/cimport.h"
#include "Libs/Assimp/include/scene.h"
#include "Libs/Assimp/include/postprocess.h"
#pragma comment (lib, "Libs/Assimp/libx86/assimp.lib")

ModuleImporter::ModuleImporter(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleImporter::~ModuleImporter()
{

}

bool ModuleImporter::Start()
{
	bool ret = true;

	return ret;
}

bool ModuleImporter::CleanUp()
{

	return true;
}