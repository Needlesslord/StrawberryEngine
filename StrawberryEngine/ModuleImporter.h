#ifndef __ModuleImporter_H__
#define __ModuleImporter_H__

#include "Module.h"
#include "Globals.h"

class TextureImporter;
class MeshImporter;
class GameObject;
class Texture;
class Mesh;


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