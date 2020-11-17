#ifndef __MeshImporter_H__
#define __MeshImporter_H__

#include "Globals.h"
#include "Mesh.h"
#include "Importer.h"
#include "scene.h"

class GameObject;
class Mesh;

class MeshImporter : public Importer
{
public:
	MeshImporter();
	~MeshImporter();

	bool Start();
	bool CleanUp();

	void Import();
	uint64 Save(Mesh* ourMesh, char** fileBuffer);
	bool Load(const char* fileBuffer, Mesh* ourMesh);

	GameObject* LoadMesh(const char* path);
	void RecursiveLoad(const aiScene* scene, GameObject* ret, const char* path, aiNode* node);

public:

	int meshIterator = 0;

};

#endif // __MeshImporter_H__