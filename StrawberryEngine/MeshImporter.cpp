#include "Application.h"
#include "MeshImporter.h"

#include "GameObject.h"
#include "Importer.h"
//#include "Mesh.h"


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

GameObject* MeshImporter::LoadMesh(const char* path)
{
	GameObject* ret = new GameObject();
	App->scene_intro->gameObjectList.push_back(ret);
	App->scene_intro->everyGameObjectList.push_back(ret);

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);


	if (scene == nullptr)
	{
		LOG("Scene was null");
		return ret;
	}

	aiVector3D translation, scaling;
	aiQuaternion rotation;
	scene->mRootNode->mTransformation.Decompose(scaling, rotation, translation);
	vec3 pos(translation.x, translation.y, translation.z);
	ret->position = pos;
	vec3 scale(scaling.x, scaling.y, scaling.z);
	ret->scale = scale;
	//const Quat rotationMGL(rotation.x, rotation.y, rotation.z, rotation.w); //btw, MGL is for MathGeoLib
	//ret->rotationQuat = rotationMGL;

	if (scene != nullptr && scene->HasMeshes())
	{
		aiNode* node = scene->mRootNode;
		for (uint i = 0; i < scene->mRootNode->mNumChildren; i++)
		{
			if (node != nullptr)
				RecursiveLoad(scene, ret, path, node->mChildren[i]);
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

void MeshImporter::RecursiveLoad(const aiScene* scene, GameObject* ret, const char* path, aiNode* node)
{
	// Use scene->mNumMeshes to iterate on scene->mMeshes array
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		char* name = strdup(node->mName.C_Str());
		GameObject* ourGO = new GameObject(name);
		App->scene_intro->everyGameObjectList.push_back(ourGO);
		ret->AddChild(ourGO);
		ourGO->meshComponent->parent = ourGO;

		LOG("%s is now a child of %s", ourGO->name, ret->name);


		aiVector3D translation, scaling;
		aiQuaternion rotation;
		node->mTransformation.Decompose(scaling, rotation, translation);
		vec3 pos(translation.x, translation.y, translation.z);
		ourGO->position = pos;
		vec3 scale(scaling.x, scaling.y, scaling.z);
		scale /= 100;
		ourGO->scale = scale;

		ourGO->isMoved = true;

		aiMesh* shortcut = scene->mMeshes[node->mMeshes[i]];//this is crazy long

		ourGO->meshComponent->num_vertex = shortcut->mNumVertices;
		ourGO->meshComponent->vertex = new vec3[ourGO->meshComponent->num_vertex * 3];
		memcpy(ourGO->meshComponent->vertex, shortcut->mVertices, sizeof(float) * ourGO->meshComponent->num_vertex * 3);
		LOG("New mesh with %d vertices", ourGO->meshComponent->num_vertex);

		// copy faces
		if (shortcut->HasFaces())
		{
			ourGO->meshComponent->num_index = shortcut->mNumFaces * 3;
			ourGO->meshComponent->index = new uint[ourGO->meshComponent->num_index]; // assume each face is a triangle
			for (uint j = 0; j < shortcut->mNumFaces; j++)
			{
				if (shortcut->mFaces[j].mNumIndices != 3)
				{
					LOG("WARNING, geometry face with != 3 indices!");
				}
				else
				{
					memcpy(&ourGO->meshComponent->index[j * 3], shortcut->mFaces[j].mIndices, 3 * sizeof(uint));
				}
			}
		}

		if (shortcut->HasTextureCoords(0))
		{
			ourGO->meshComponent->tex_coord = new float[ourGO->meshComponent->num_vertex * 2];

			for (int j = 0; j < ourGO->meshComponent->num_vertex; j++)
			{
				ourGO->meshComponent->tex_coord[j * 2] = shortcut->mTextureCoords[0][j].x;
				ourGO->meshComponent->tex_coord[(j * 2) + 1] = shortcut->mTextureCoords[0][j].y;
			}

			glGenBuffers(1, (GLuint*) & (ourGO->meshComponent->id_tex_coord));
			glBindBuffer(GL_ARRAY_BUFFER, ourGO->meshComponent->id_tex_coord);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ourGO->meshComponent->num_vertex * 2, ourGO->meshComponent->tex_coord, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			ourGO->meshComponent->hasTex_coords = true;
		}

		if (shortcut->HasNormals())
		{
			ourGO->meshComponent->normals = new vec3[shortcut->mNumVertices];
			memcpy(ourGO->meshComponent->normals, shortcut->mNormals, sizeof(vec3) * shortcut->mNumVertices);
			ourGO->meshComponent->hasNormals = true;
		}

		ourGO->meshComponent->path = path;

		App->scene_intro->meshesList.push_back(ourGO->meshComponent);

		
	}

	for (uint i = 0; i < node->mNumChildren; i++)
	{
		if (node != nullptr)
			RecursiveLoad(scene, ret, path, node->mChildren[i]);
	}
}

void MeshImporter::Import()
{
	//importer = new
}

uint64 MeshImporter::Save(Mesh* ourMesh, char** fileBuffer)
{
	// amount of indices / vertices / colors / normals / texture_coords / AABB
	uint ranges[2] = { ourMesh->num_index, ourMesh->num_vertex };
	uint size = sizeof(ranges) + sizeof(uint) * ourMesh->num_index + sizeof(float) * ourMesh->num_vertex * 3;
	*fileBuffer = new char[size]; // Allocate
	char* cursor = *fileBuffer;

	uint bytes = sizeof(ranges); // First store ranges
	memcpy(cursor, ranges, bytes);
	cursor += bytes;

	// Store indices
	bytes = sizeof(uint) * ourMesh->num_index;
	memcpy(cursor, ourMesh->index, bytes);
	cursor += bytes;

	// Store vertices
	bytes = sizeof(uint) * ourMesh->num_vertex;
	memcpy(cursor, ourMesh->vertex, bytes);
	cursor += bytes;

	if (ourMesh->hasNormals)
	{
		// Store normals
		bytes = sizeof(uint) * ourMesh->num_vertex;
		memcpy(cursor, ourMesh->normals, bytes);
		cursor += bytes;
	}

	if (ourMesh->hasTex_coords)
	{
		// Store tex coords
		bytes = sizeof(uint) * ourMesh->num_vertex * 2;
		memcpy(cursor, ourMesh->tex_coord, bytes);
		cursor += bytes;
	}

	//TODO: Save AABB


	uint a = App->fileSystem->Save("/Library/Meshes/test01.sem", *fileBuffer, size);

	//RELEASE(*fileBuffer);

	return size;
}

bool MeshImporter::Load(const char* fileBuffer, Mesh* ourMesh)
{
	const char* cursor = fileBuffer;
	// amount of indices / vertices / colors / normals / texture_coords
	uint ranges[5];
	uint bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);
	cursor += bytes;
	ourMesh->num_index = ranges[0];
	ourMesh->num_vertex = ranges[1];

	// Load indices
	bytes = sizeof(uint) * ourMesh->num_index;
	ourMesh->index = new uint[ourMesh->num_index];
	memcpy(ourMesh->index, cursor, bytes);
	cursor += bytes;

	// Load vertices
	bytes = sizeof(uint) * ourMesh->num_vertex * 3;
	ourMesh->vertex = new vec3[ourMesh->num_vertex * 3]; // Not sure about this
	memcpy(ourMesh->vertex, cursor, bytes);
	cursor += bytes;

	// Load normals
	bytes = sizeof(uint) * ourMesh->num_vertex * 3;
	ourMesh->normals = new vec3[ourMesh->num_vertex * 3]; // Not sure about this
	memcpy(ourMesh->normals, cursor, bytes);
	cursor += bytes;

	// Load tex coords
	bytes = sizeof(uint) * ourMesh->num_vertex * 2;
	ourMesh->tex_coord = new float[ourMesh->num_vertex * 2]; // Not sure about this
	memcpy(ourMesh->tex_coord, cursor, bytes);

	//TODO: Load AABB

	return true;
}
