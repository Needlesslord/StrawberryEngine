#ifndef __ModuleImporter_H__
#define __ModuleImporter_H__

#include "Module.h"
#include "Globals.h"

class Mesh
{
public:
	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = nullptr;
	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = nullptr;
};

class ModuleImporter : public Module
{
public:
	ModuleImporter(Application* app, bool start_enabled = true);
	~ModuleImporter();

	bool Init();
	bool Start();
	bool CleanUp();

	void Load(char* path, std::list<Mesh*>* list);

};

#endif // __ModuleImporter_H__