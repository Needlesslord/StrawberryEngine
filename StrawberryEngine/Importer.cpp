#include "Application.h"
#include "Importer.h"
#include "GameObject.h"
#include "Libs/Assimp/include/cimport.h"
#include "Libs/Assimp/include/scene.h"
#include "Libs/Assimp/include/postprocess.h"

Importer::Importer()
{

}

Importer::~Importer()
{

}

void Importer::Import(const aiMaterial* material, Mesh* ourMaterial)
{

}

bool Importer::Save()
{
	return true;
}
bool Importer::Load()
{
	return true;
}