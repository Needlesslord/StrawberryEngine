#ifndef __MeshImporter_H__
#define __MeshImporter_H__

#include "Globals.h"
#include "Mesh.h"
#include "Importer.h"

class GameObject;
class Mesh;

class MeshImporter : public Importer
{
public:
	MeshImporter();
	~MeshImporter();

	bool Start();
	bool CleanUp();

	GameObject* Load(const char* path);

public:

	int meshIterator = 0;

};

#endif // __MeshImporter_H__