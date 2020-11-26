#include "TextureImporter.h"
#include "Application.h"
#include "ModuleFileSystem.h"

#include "Libs/DevIL/include/IL/il.h"
#include "Libs/DevIL/include/IL/ilu.h"
#include "Libs/DevIL/include/IL/ilut.h"

#pragma comment ( lib, "Libs/DevIL/libx86/DevIL.lib" )
#pragma comment ( lib, "Libs/DevIL/libx86/ILU.lib" )
#pragma comment ( lib, "Libs/DevIL/libx86/ILUT.lib" )

TextureImporter::TextureImporter()
{
	
}

TextureImporter::~TextureImporter()
{

}

bool TextureImporter::Init()
{
	bool ret = true;

	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION || iluGetInteger(ILU_VERSION_NUM) < ILU_VERSION || ilutGetInteger(ILUT_VERSION_NUM) < ILUT_VERSION)
	{
		LOG("DevIL version is different...exiting!");

		ret = false;
	}
	else
	{
		LOG("Initializing DevIL");
		
		
		ilInit();
		ilutRenderer(ILUT_OPENGL);
	}

	return ret;
}

bool TextureImporter::Start()
{
	bool ret = true;

	
	houseTexture = LoadTexture("Assets/Textures/Baker_House.png");



	return ret;
}

void TextureImporter::Import()
{
	//importer = new
}

uint64 TextureImporter::Save(const Texture* ourTexture, char** fileBuffer)
{
	ILuint size;
	ILubyte* data;
	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
	size = ilSaveL(IL_DDS, nullptr, 0);

	if (size > 0) 
	{
		data = new ILubyte[size];
		if (ilSaveL(IL_DDS, data, size) > 0) 
			*fileBuffer = (char*)data;

		App->fileSystem->Save(ourTexture->name, fileBuffer, size);
		RELEASE_ARRAY(data);
		//RELEASE_ARRAY(fileBuffer);
	}

	return size;
}

bool TextureImporter::Load(const char* fileBuffer, Texture* ourTexture)
{
	return true;
}


Texture* TextureImporter::LoadTexture(const char* path)
{
	if (ilLoadImage(path))
	{
		LOG("Image path loaded properly");
	}
	else
	{
		LOG("Couldn't load image");
		return nullptr;
	}

	Texture* ret = new Texture;
	ret->path = path;

	std::string name(path);
	uint a = name.find_last_of("\\/");
	name = name.substr(a + 1);
	char* n = new char[30];
	strcpy(n, name.c_str());
	ret->name = n;

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

	LOG(ret->name);
	App->importer->AddTexture(ret);
	
	//char* buffer = nullptr;
	//Save(ret, &buffer);

	return ret;
}