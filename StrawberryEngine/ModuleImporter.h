#ifndef __ModuleImporter_H__
#define __ModuleImporter_H__

#include "Module.h"
#include "Globals.h"

class TextureImporter;
class MeshImporter;
class GameObject;
class Texture;
class Mesh;

class Importer
{

public:
	Importer() {};
	virtual ~Importer() {};

	virtual bool Init()
	{ 
		return true; 
	}
	virtual bool Start()
	{
		return true;
	}
	virtual update_status PreUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}
	virtual update_status PostUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}
	virtual bool CleanUp()
	{
		return true;
	}

	void Import();
	bool Save();
	bool Load();

public:

};

class ModuleImporter : public Module
{
public:
	ModuleImporter(Application* app, bool start_enabled = true);
	~ModuleImporter();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
	void AddGameObject(GameObject* go)const;
	void AddMesh(Mesh* mesh)const;
	void AddTexture(Texture* tex)const;

public:

	TextureImporter* textureImporter;
	MeshImporter* meshImporter;

};

#endif // __ModuleImporter_H__