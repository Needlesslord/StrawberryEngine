#include "Globals.h"
#include "Importer.h"
#include "TextureComponent.h"

#include "Libs/DevIL/include/IL/il.h"

class TextureImporter : public Importer
{
public:

	TextureImporter();
	virtual~TextureImporter();

	bool Init();
	bool Start();
	void Import();
	uint64 Save(const TextureComponent* ourTexture, char** fileBuffer);
	bool Load(const char* fileBuffer, TextureComponent* ourTexture);

	TextureComponent* LoadTexture(const char* path);

public:

	TextureComponent* defaultTexture = nullptr;
	TextureComponent* houseTexture = nullptr;

};
