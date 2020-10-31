#ifndef __ModuleImporter_H__
#define __ModuleImporter_H__

#include "Module.h"
#include "Globals.h"
#include "Libs/DevIL/include/IL/il.h"
#include "vector3.h"

class GameObject;

class Mesh
{
public:
	Mesh() {};
	~Mesh() {};
public:

	char* name = nullptr;
	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = nullptr;
	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = nullptr;
	uint id_tex_coord = 0;
	float* tex_coord = nullptr;
	aiVector3D* normals = nullptr;

	bool isSelected = false;

	uint textureNumber = 999;

	vec3 position = { 0,0,0 }; 
	vec3 rotation = { 0,0,0 };
	vec3 scale = { 0,0,0 };

	bool isDrawEnabled = true;
	bool isFaceNormalsEnabled = false;
	bool isVertexNormalsEnabled = false;

};

class Texture
{
public:
	const char* path = nullptr;
	char* name = nullptr;
	uint id;
	uint w;
	uint h;

	uint textureIterator;
};

class ModuleImporter : public Module
{
public:
	ModuleImporter(Application* app, bool start_enabled = true);
	~ModuleImporter();

	bool Init();
	bool Start();
	bool CleanUp();

	Texture* LoadTexture(const char* path);

	GameObject* Load(const char* path);

public:

	Texture* defaultTexture = nullptr;
	Texture* houseTexture = nullptr;
	std::list<Texture*> textureList;

	int textureIterator = 1;
	int meshIterator = 0;

};

#endif // __ModuleImporter_H__