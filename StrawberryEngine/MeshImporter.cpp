#include "Application.h"
#include "Module.h"
#include "MeshImporter.h"
#include "GameObject.h"
#include "ModuleImporter.h"
#include "TextureImporter.h"
#include "ModuleUI.h"

#include "Libs/Assimp/include/cimport.h"
#include "Libs/Assimp/include/scene.h"
#include "Libs/Assimp/include/postprocess.h"
#pragma comment (lib, "Libs/Assimp/libx86/assimp.lib")

#include <math.h>

MeshImporter::MeshImporter()
{

}

MeshImporter::~MeshImporter()
{

}

bool MeshImporter::Start()
{
	bool ret = true;

	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
	LOG("Initializing Assimp");

	return ret;
}

bool MeshImporter::CleanUp()
{
	aiDetachAllLogStreams();

	//ilDeleteImages(1, &ImageName);

	return true;
}

GameObject* MeshImporter::Load(const char* path)
{
	GameObject* ret = new GameObject();

	importer->AddGameObject(ret);

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			Mesh* ourMesh = new Mesh();
			ourMesh->num_vertex = scene->mMeshes[i]->mNumVertices;
			ourMesh->vertex = new vec3[ourMesh->num_vertex * 3];
			memcpy(ourMesh->vertex, scene->mMeshes[i]->mVertices, sizeof(float) * ourMesh->num_vertex * 3);
			LOG("New mesh with %d vertices", ourMesh->num_vertex);

			// copy faces
			if (scene->mMeshes[i]->HasFaces())
			{
				ourMesh->num_index = scene->mMeshes[i]->mNumFaces * 3;
				ourMesh->index = new uint[ourMesh->num_index]; // assume each face is a triangle
				for (uint j = 0; j < scene->mMeshes[i]->mNumFaces; j++)
				{
					if (scene->mMeshes[i]->mFaces[j].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&ourMesh->index[j * 3], scene->mMeshes[i]->mFaces[j].mIndices, 3 * sizeof(uint));
					}
				}
			}

			if (scene->mMeshes[i]->HasTextureCoords(0))
			{
				ourMesh->tex_coord = new float[ourMesh->num_vertex * 2];

				for (int j = 0; j < ourMesh->num_vertex; j++)
				{
					ourMesh->tex_coord[j * 2] = scene->mMeshes[i]->mTextureCoords[0][j].x;
					ourMesh->tex_coord[(j * 2) + 1] = scene->mMeshes[i]->mTextureCoords[0][j].y;
				}

				glGenBuffers(1, (GLuint*) & (ourMesh->id_tex_coord));
				glBindBuffer(GL_ARRAY_BUFFER, ourMesh->id_tex_coord);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ourMesh->num_vertex * 2, ourMesh->tex_coord, GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
			
			if (scene->mMeshes[i]->HasNormals())
			{
				ourMesh->normals = new vec3[scene->mMeshes[i]->mNumVertices];
				memcpy(ourMesh->normals, scene->mMeshes[i]->mNormals, sizeof(vec3) * scene->mMeshes[i]->mNumVertices);
				ourMesh->hasNormals = true;
			}

			if (scene->mMeshes[i]->HasPositions())
			{
				aiVector3D translation, scaling;
				aiQuaternion rotation;
				scene->mRootNode->mTransformation.Decompose(scaling, rotation, translation);
				vec3 pos(translation.x, translation.y, translation.z);
				ourMesh->position = pos;
				vec3 scale(scaling.x, scaling.y, scaling.z);
				ourMesh->scale = scale;
				//Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);
			}

			char* charName = new char[12];
			std::string stringName = ("New Mesh "+ std::to_string(meshIterator));
			strcpy(charName, stringName.c_str());
			ourMesh->name = charName;
			meshIterator++;

			ourMesh->path = path;
			
			ret->AddChild(ourMesh);
			LOG("%s is now a child of %s", ourMesh->name, ret->name);
			//App->ui->AddConsoleOutput("%s is now a child of %s", ourMesh->name, ret->name);
			importer->AddMesh(ourMesh);

		}
		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene % s", path);
		return nullptr;
	}
		
	return ret;
}

