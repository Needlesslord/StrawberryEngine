#ifndef __MeshImporter_H__
#define __MeshImporter_H__

#include "Module.h"
#include "Globals.h"
#include "vector3.h"
#include "ModuleImporter.h"

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
	vec3* vertex = nullptr;
	uint id_tex_coord = 0;
	float* tex_coord = nullptr;

	bool hasNormals = false;
	vec3* normals = nullptr;

	bool isSelected = false;

	uint textureNumber = 999;

	vec3 position = { 0,0,0 }; 
	vec3 rotation = { 0,0,0 };
	float scale = 1.0f;

	bool isDrawEnabled = true;
	bool isFaceNormalsEnabled = false;
	bool isVertexNormalsEnabled = false;

	const char* path = nullptr;

};



class MeshImporter : public Importer
{
public:
	MeshImporter();
	~MeshImporter();

	bool Start()override;
	bool CleanUp()override;

	GameObject* Load(const char* path);

public:

	ModuleImporter* importer;
	int meshIterator = 0;

};

#endif // __MeshImporter_H__