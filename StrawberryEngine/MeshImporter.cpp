#include "Application.h"
#include "MeshImporter.h"
#include "TextureImporter.h"
#include "GameObject.h"
#include "Importer.h"


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

	test = App->scene_intro->AddGameObject("test");

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
	App->scene_intro->rootNode->children.push_back(ret);
	ret->parent = App->scene_intro->rootNode;
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

	float3 pos(translation.x, translation.y, translation.z);
	ret->position = pos;

	float3 scale(scaling.x, scaling.y, scaling.z);
	ret->scale = scale;

	const Quat rotationMGL(rotation.x, rotation.y, rotation.z, rotation.w); //btw, MGL is for MathGeoLib
	ret->rotationQuat = rotationMGL;
	ret->rotation = ret->rotationQuat.ToEulerXYZ() * RADTODEG;

	ret->previousRotation = ret->rotation;
	ret->UpdateLocalTransform();
	ret->UpdateGlobalTransform();

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
		if (node->mNumMeshes > 1)
			i++;
		char* name = strdup(node->mName.C_Str());
		GameObject* ourGO = new GameObject(name);
		App->scene_intro->everyGameObjectList.push_back(ourGO);
		ret->AddChild(ourGO);

		ourGO->meshComponent = (ComponentMesh*) ourGO->AddComponent(Component::TYPE_MESH);
		ourGO->meshComponent->mesh = new Mesh;
		ourGO->meshComponent->name = name;

		LOG("%s is now a child of %s", ourGO->name.c_str(), ret->name.c_str());

		aiVector3D translation, scaling;
		aiQuaternion rotation;
		node->mTransformation.Decompose(scaling, rotation, translation);
		float3 pos(translation.x, translation.y, translation.z);
		ourGO->position = pos;
		float3 scale(scaling.x, scaling.y, scaling.z);

		if (scale.x == 100 || scale.y == 100 || scale.z == 100)
			scale /= 100;

		ourGO->scale = scale;
		const Quat rotationMGL(rotation.x, rotation.y, rotation.z, rotation.w); //btw, MGL is for MathGeoLib
		ourGO->rotationQuat = rotationMGL;
		ourGO->rotation = ourGO->rotationQuat.ToEulerXYZ() * RADTODEG;
		ourGO->previousRotation = ourGO->rotation;
		ourGO->isMoved = true;

		aiMesh* shortcut = scene->mMeshes[node->mMeshes[i]];//this is crazy long

		aiMaterial* texture = scene->mMaterials[shortcut->mMaterialIndex];
		uint numTextures = texture->GetTextureCount(aiTextureType_DIFFUSE);
		aiString texPath;
		texture->GetTexture(aiTextureType_DIFFUSE, 0, &texPath);
		Texture* ourTexture;
		if (texPath.C_Str() != nullptr && texPath.length > 0)
		{
			std::string texPathString = App->fileSystem->NormalizePath(texPath.C_Str());
			texPathString = texPathString.substr(2);
			texPathString = "Assets" + texPathString;
			
			ourTexture = App->importer->textureImporter->LoadTexture(texPathString.c_str());
			if (ourTexture == nullptr)
			{

				if (App->fileSystem->HasExtension(texPathString.c_str(), "tga") || App->fileSystem->HasExtension(texPathString.c_str(), "TGA"))
				{
					std::string ext = "png";
					std::string pathChanged = App->fileSystem->ChangeExtension(texPathString.c_str(), ext);

					ourTexture = App->importer->textureImporter->LoadTexture(pathChanged.c_str());
					if (ourTexture != nullptr)
					{
						ourGO->textureComponent = (ComponentTexture*)ourGO->AddComponent(Component::TYPE_TEXTURE);
						ourGO->textureComponent->texture = ourTexture;
					}
				}
			}
			else
			{
				ourGO->textureComponent = (ComponentTexture*)ourGO->AddComponent(Component::TYPE_TEXTURE);
				ourGO->textureComponent->texture = ourTexture;
			}
		}
		
		Mesh* ourMesh = ourGO->meshComponent->mesh;
		ourMesh->name = ourGO->meshComponent->name;
		// Vertex
		ourMesh->num_vertex = shortcut->mNumVertices;
		ourMesh->vertex = new float3[ourMesh->num_vertex * 3];
		memcpy(ourMesh->vertex, shortcut->mVertices, sizeof(float) * ourMesh->num_vertex * 3);
		LOG("New mesh with %d vertices", ourMesh->num_vertex);


		// AABB
		ourMesh->localAABB.SetNegativeInfinity();
		ourMesh->localAABB.Enclose((float3*)ourMesh->vertex, ourMesh->num_vertex);


		// Copy faces
		if (shortcut->HasFaces())
		{
			ourMesh->num_index = shortcut->mNumFaces * 3;
			ourMesh->index = new uint[ourMesh->num_index]; // assume each face is a triangle
			for (uint j = 0; j < shortcut->mNumFaces; j++)
			{
				if (shortcut->mFaces[j].mNumIndices != 3)
				{
					LOG("WARNING, geometry face with != 3 indices!");
				}
				else
				{
					memcpy(&ourMesh->index[j * 3], shortcut->mFaces[j].mIndices, 3 * sizeof(uint));
				}
			}
		}


		// UVs
		if (shortcut->HasTextureCoords(0))
		{
			ourMesh->tex_coord = new float[ourMesh->num_vertex * 2];

			for (int j = 0; j < ourMesh->num_vertex; j++)
			{
				ourMesh->tex_coord[j * 2] = shortcut->mTextureCoords[0][j].x;
				ourMesh->tex_coord[(j * 2) + 1] = shortcut->mTextureCoords[0][j].y;
			}

			glGenBuffers(1, (GLuint*) & (ourMesh->id_tex_coord));
			glBindBuffer(GL_ARRAY_BUFFER, ourMesh->id_tex_coord);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ourMesh->num_vertex * 2, ourMesh->tex_coord, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			ourMesh->hasTex_coords = true;
		}


		// Normals
		if (shortcut->HasNormals())
		{
			ourMesh->normals = new float3[shortcut->mNumVertices];
			memcpy(ourMesh->normals, shortcut->mNormals, sizeof(float3) * shortcut->mNumVertices);
			ourMesh->hasNormals = true;
		}

		App->scene_intro->meshesList.push_back(ourGO->meshComponent->mesh);

		char* buffer;
		Save(ourGO->meshComponent->mesh, &buffer, ourGO->meshComponent->name); 


		/*char* goname;
		if (testIterator == 0)
		{
			goname = "TestingLoad 0";
		}
		else
			goname = "TestingLoad 1";
		testIterator++;
		GameObject* testingLoadGO = new GameObject(goname);
		App->scene_intro->everyGameObjectList.push_back(testingLoadGO);
		test->AddChild(testingLoadGO);
		testingLoadGO->meshComponent->parent = testingLoadGO;

		Load(buffer, testingLoadGO->meshComponent);
		testingLoadGO->position.x += 4;
		testingLoadGO->isMoved = true;
		App->scene_intro->meshesList.push_back(testingLoadGO->meshComponent);
		App->renderer3D->GenerateBuffers();*/

		RELEASE(buffer);
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

uint64 MeshImporter::Save(Mesh* ourMesh, char** fileBuffer, std::string name)
{
	// amount of indices / vertices / colors / normals / texture_coords / AABB
	uint ranges[4] = { ourMesh->num_index, ourMesh->num_vertex, ourMesh->num_vertex/* -(Normals)- */, ourMesh->num_vertex /* -(UVs)- */ };
	uint size = sizeof(ranges) + sizeof(uint)
		+ sizeof(uint) * ourMesh->num_index 
		+ sizeof(float) * ourMesh->num_vertex * 3 
		+ sizeof(float) * ourMesh->num_vertex * 3 /* -(Normals)- */
		+ sizeof(float) * ourMesh->num_vertex * 2 /* -(UVs)- */;

	*fileBuffer = new char[size]; // Allocate
	char* cursor = *fileBuffer;

	// First store ranges
	uint bytes = sizeof(ranges); 
	memcpy(cursor, ranges, bytes);
	cursor += bytes;

	// Store indices
	bytes = sizeof(uint) * ourMesh->num_index;
	memcpy(cursor, ourMesh->index, bytes);
	cursor += bytes;

	// Store vertices
	bytes = sizeof(float) * ourMesh->num_vertex * 3;
	memcpy(cursor, ourMesh->vertex, bytes);
	cursor += bytes;


	//if (ourMesh->hasNormals)
	//{
		// Store normals
		bytes = sizeof(float) * ourMesh->num_vertex * 3;
		memcpy(cursor, ourMesh->normals, bytes);
		cursor += bytes;
	//}

	if (ourMesh->hasTex_coords)
	{
		// Store tex coords
		bytes = sizeof(float) * ourMesh->num_vertex * 2;
		memcpy(cursor, ourMesh->tex_coord, bytes);
		cursor += bytes;
	}

	//TODO: Save AABB

	char file[250]; 
	sprintf_s(file, 250, "%s%s.sem", MESHES_PATH, name.c_str()/*Should be the mesh's name*/);

	App->fileSystem->Save(file, *fileBuffer, size);

	//RELEASE(*fileBuffer);

	return size;
}

bool MeshImporter::Load(const char* fileBuffer, Mesh* ourMesh)
{
	const char* cursor = fileBuffer;
	// amount of indices / vertices / colors / normals / texture_coords
	uint ranges[4];
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
	bytes = sizeof(float) * ourMesh->num_vertex * 3;
	ourMesh->vertex = new float3[ourMesh->num_vertex]; // Not sure about this
	memcpy(ourMesh->vertex, cursor, bytes);
	cursor += bytes;

	// Load normals
	bytes = sizeof(float) * ourMesh->num_vertex * 3;
	ourMesh->normals = new float3[ourMesh->num_vertex]; // Not sure about this !!!!!!!!!!!!
	memcpy(ourMesh->normals, cursor, bytes);
	cursor += bytes;
	ourMesh->hasNormals = true; // IDK

	// Load tex coords
	bytes = sizeof(float) * ourMesh->num_vertex * 2;
	ourMesh->tex_coord = new float[ourMesh->num_vertex * 2]; // Not sure about this
	memcpy(ourMesh->tex_coord, cursor, bytes);
	ourMesh->hasTex_coords = true; // IDK

	glGenBuffers(1, (GLuint*) & (ourMesh->id_tex_coord));
	glBindBuffer(GL_ARRAY_BUFFER, ourMesh->id_tex_coord);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ourMesh->num_vertex * 2, ourMesh->tex_coord, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//TODO: Load AABB

	return true;
}

bool MeshImporter::HasAssimp(const char* name) const
{
	std::string ext = "Assimp";
	std::string a(name);
	int ret = a.find(ext.c_str());

	return !(ret == -1);
}