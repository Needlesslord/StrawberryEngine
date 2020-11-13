#include "Globals.h"
#include "ModuleImporter.h"

#include "Libs/DevIL/include/IL/il.h"

class Texture
{
public:
	uint GetId() { return id; }
public:
	const char* path = nullptr;
	char* name = nullptr;
	uint id;
	uint w;
	uint h;

	uint textureIterator;
};

class TextureImporter : public Importer
{
public:
	TextureImporter();
	virtual~TextureImporter();

	bool Init()override;
	bool Start()override;
	void Import();
	bool Save();
	bool Load();

	Texture* LoadTexture(const char* path);

public:

	ModuleImporter* importer;

	Texture* defaultTexture = nullptr;
	Texture* houseTexture = nullptr;

	int textureIterator = 1;
};
