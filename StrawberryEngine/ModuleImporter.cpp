#include "Application.h"
#include "ModuleImporter.h"
#include "GameObject.h"

#include "Libs/Assimp/include/cimport.h"
#include "Libs/Assimp/include/scene.h"
#include "Libs/Assimp/include/postprocess.h"
#pragma comment (lib, "Libs/Assimp/libx86/assimp.lib")

#include "Libs/DevIL/include/IL/il.h"
#include "Libs/DevIL/include/IL/ilu.h"
#include "Libs/DevIL/include/IL/ilut.h"

#pragma comment ( lib, "Libs/DevIL/libx86/DevIL.lib" )
#pragma comment ( lib, "Libs/DevIL/libx86/ILU.lib" )
#pragma comment ( lib, "Libs/DevIL/libx86/ILUT.lib" )

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

	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION || iluGetInteger(ILU_VERSION_NUM) < ILU_VERSION || ilutGetInteger(ILUT_VERSION_NUM) < ILUT_VERSION)
	{
		LOG("DevIL version is different...exiting!");
		return false;
	}
	else
	{
		LOG("Initializing DevIL");

		ilInit();
		ilutRenderer(ILUT_OPENGL);
	}

	return ret;
}

bool ModuleImporter::Start()
{
	bool ret = true;

	houseTexture = LoadTexture("Assets/Baker_House.png");

	return ret;
}

bool ModuleImporter::CleanUp()
{
	aiDetachAllLogStreams();

	//ilDeleteImages(1, &ImageName);

	return true;
}

GameObject* ModuleImporter::Load(char* path, char* name)
{
	GameObject* ret = new GameObject(name);
	App->scene_intro->gameObjectList.push_back(ret);

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			Mesh* ourMesh = new Mesh();
			ourMesh->num_vertex = scene->mMeshes[i]->mNumVertices;
			ourMesh->vertex = new float[ourMesh->num_vertex * 3];
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
				ourMesh->num_tex_coord = ourMesh->num_vertex;
				ourMesh->tex_coord = new float[ourMesh->num_tex_coord * 2];

				for (int j = 0; j < ourMesh->num_tex_coord; j++)
				{
					ourMesh->tex_coord[j * 2] = scene->mMeshes[i]->mTextureCoords[0][j].x;
					ourMesh->tex_coord[(j * 2) + 1] = scene->mMeshes[i]->mTextureCoords[0][j].y;
				}

				glGenBuffers(1, (GLuint*) & (ourMesh->id_tex_coord));
				glBindBuffer(GL_ARRAY_BUFFER, ourMesh->id_tex_coord);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ourMesh->num_tex_coord * 2, ourMesh->tex_coord, GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}

			ret->AddChild(ourMesh);
			LOG("%s is now a child of %s", ourMesh->name, ret->name);
			App->scene_intro->meshesList.push_back(ourMesh);
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

Texture* ModuleImporter::LoadTexture(char* path)
{
	Texture* ret = new Texture;

	//ILuint ImageName;
	//ilGenImages(1, &ImageName);
	//ilBindImage(ImageName);

	if (ilLoadImage(path))
	{
		LOG("Image path loaded properly");
		ret->path = path;
	}
	else
	{
		LOG("Couldn't load image");
		return ret;
	}

	ret->id = ilutGLBindTexImage();
	ret->w = ilGetInteger(IL_IMAGE_WIDTH);
	ret->h = ilGetInteger(IL_IMAGE_HEIGHT);

	ILinfo ImgInfo;
	iluGetImageInfo(&ImgInfo);

	if (ImgInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		iluFlipImage();

	uint id_texture = 0;
	glGenTextures(1, (GLuint*)&id_texture);
	glBindTexture(GL_TEXTURE_2D, id_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ret->w, ret->h, 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	ret->id = id_texture;

	textureList.push_back(ret);

	return ret;
}