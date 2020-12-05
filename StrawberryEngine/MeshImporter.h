#ifndef __MeshImporter_H__
#define __MeshImporter_H__

#include "Globals.h"
#include "ComponentMesh.h"
#include "Importer.h"
#include "scene.h"

class GameObject;

class MeshImporter : public Importer
{
public:
	MeshImporter();
	~MeshImporter();

	bool Start();
	bool CleanUp();

	void Import();
	uint64 Save(Mesh* ourMesh, char** fileBuffer, std::string name);
	bool Load(const char* fileBuffer, Mesh* ourMesh);

	GameObject* LoadMesh(const char* path);
	void RecursiveLoad(const aiScene* scene, GameObject* ret, const char* path, aiNode* node);

	bool HasAssimp(const char* name) const;

public:

	int meshIterator = 0;
	GameObject* test = nullptr;
	int testIterator = 0;
};

#endif // __MeshImporter_H__