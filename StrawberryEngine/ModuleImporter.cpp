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

bool ModuleImporter::Init()
{
	bool ret = true;

	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return ret;
}

bool ModuleImporter::Start()
{
	bool ret = true;

	return ret;
}

bool ModuleImporter::CleanUp()
{
	aiDetachAllLogStreams();
	return true;
}

void ModuleImporter::Load(char* path, std::list<Mesh*>* list)
{
	//std::list<Mesh*> ret;

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			Mesh ourMesh;
			ourMesh.num_vertex = scene->mMeshes[i]->mNumVertices;
			ourMesh.vertex = new float[ourMesh.num_vertex * 3];
			memcpy(ourMesh.vertex, scene->mMeshes[i]->mVertices, sizeof(float) * ourMesh.num_vertex * 3);
			LOG("New mesh with %d vertices", ourMesh.num_vertex);

			// copy faces
			if (scene->mMeshes[i]->HasFaces())
			{
				ourMesh.num_index = scene->mMeshes[i]->mNumFaces * 3;
				ourMesh.index = new uint[ourMesh.num_index]; // assume each face is a triangle
				for (uint j = 0; j < scene->mMeshes[i]->mNumFaces; j++)
				{
					if (scene->mMeshes[i]->mFaces[j].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&ourMesh.index[j * 3], scene->mMeshes[i]->mFaces[j].mIndices, 3 * sizeof(uint));
					}
				}
			}
			list->push_back(&ourMesh);
		}
		aiReleaseImport(scene);
	}
	else
		LOG("Error loading scene % s", path);
}
