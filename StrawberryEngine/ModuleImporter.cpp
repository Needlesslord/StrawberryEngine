#include "Application.h"
#include "ModuleImporter.h"
#include "TextureImporter.h"
#include "MeshImporter.h"
#include "GameObject.h"
#include "ModuleUI.h"


ModuleImporter::ModuleImporter(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	textureImporter = new TextureImporter();
	meshImporter = new MeshImporter();
}

ModuleImporter::~ModuleImporter()
{

}

bool ModuleImporter::Init()
{
	bool ret;
	
	if (textureImporter->Init() && meshImporter->Init())
	{
		ret = true;
	}

	else
	{
		ret = false;
	}

	return ret;
}

bool ModuleImporter::Start()
{
	bool ret;

	if (textureImporter->Start() && meshImporter->Start())
	{
		ret = true;
	}

	else
	{
		ret = false;
	}
	
	return ret;
}

update_status ModuleImporter::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleImporter::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleImporter::CleanUp()
{
	bool ret;

	if (textureImporter->CleanUp() && meshImporter->CleanUp())
	{
		ret = true;
	}

	else
	{
		ret = false;
	}

	return ret;
}

void ModuleImporter::AddGameObject(GameObject* go)const 
{
	App->scene_intro->gameObjectList.push_back(go);
}

void ModuleImporter::AddMesh(Mesh* mesh)const
{
	App->scene_intro->meshesList.push_back(mesh);
}

void ModuleImporter::AddTexture(Texture* tex)const
{
	App->scene_intro->textureList.push_back(tex);
}