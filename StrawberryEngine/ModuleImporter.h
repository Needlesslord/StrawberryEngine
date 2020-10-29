#ifndef __ModuleImporter_H__
#define __ModuleImporter_H__

#include "Module.h"
#include "Globals.h"
#include "Libs/DevIL/include/IL/il.h"

class GameObject;

class Mesh
{
public:
	Mesh(char* n = GAME_MESH_NAME) { name = n; };
	~Mesh() {};
public:

	char* name;
	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = nullptr;
	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = nullptr;
	uint id_tex_coord = 0;
	uint num_tex_coord = 0;
	float* tex_coord = nullptr;
	bool selected = false;

};

class Texture
{
public:
	char* path = nullptr;
	uint id;
	uint w;
	uint h;
};

class ModuleImporter : public Module
{
public:
	ModuleImporter(Application* app, bool start_enabled = true);
	~ModuleImporter();

	bool Init();
	bool Start();
	bool CleanUp();

	Texture* LoadTexture(char* path);

	GameObject* Load(char* path, char* name = GAME_OBJECT_DEFAULT_NAME);

public:

	Texture* houseTexture = nullptr;
	std::list<Texture*> textureList;

};

#endif // __ModuleImporter_H__