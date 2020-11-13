#ifndef __Importer_H__
#define __Importer_H__

#include "Globals.h"

struct aiMaterial;
class Importer
{
public:
	Importer();
	~Importer();

	void Import(const aiMaterial* material, Mesh* ourMesh);
	bool Save();
	bool Load();

};

#endif // __Importer_H__