#include "Globals.h"
#include "Importer.h"
#include "Texture.h"

#include "Libs/DevIL/include/IL/il.h"

class TextureImporter : public Importer
{
public:
	TextureImporter();
	virtual~TextureImporter();

	bool Init();
	bool Start();
	void Import();
	uint64 Save(const Texture* ourTexture, char** fileBuffer);
	bool Load(const char* fileBuffer, Texture* ourTexture);

	Texture* LoadTexture(const char* path);

public:

	Texture* defaultTexture = nullptr;
	Texture* houseTexture = nullptr;

	int textureIterator = 1;
};
