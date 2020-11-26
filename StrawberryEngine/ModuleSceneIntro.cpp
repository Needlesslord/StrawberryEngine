#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"

#include "ModuleImporter.h"
#include "MeshImporter.h"
#include "TextureImporter.h"
#include "GameObject.h"
#include "ModuleInput.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	rootNode = new GameObject("RootNode");
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	//house = App->importer->meshImporter->LoadMesh("Assets/Meshes/BakerHouse.fbx");
	street = App->importer->meshImporter->LoadMesh("Assets/Meshes/Street environment_V01.FBX");
	street->rotation = { -90,0,0 };
	street->isMoved = true;
	street->ChangeName("Street");

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	meshesList.clear();

	return true;
}

update_status ModuleSceneIntro::PreUpdate(float dt)
{
	

	// Refresh selected mesh list
	gameObjectSelected.clear();
	for (std::list<GameObject*>::iterator goToSelect = everyGameObjectList.begin(); goToSelect != everyGameObjectList.end(); goToSelect++)
	{
		if ((*goToSelect)->isSelected)
		{
			gameObjectSelected.push_back(*goToSelect);
		}
	}

	return UPDATE_CONTINUE;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{

	p = { 0.0, 1.0, 0.0, 0.0 };
	p.axis = true;

	bool needToGenBuffers = false;
	for (std::list<GameObject*>::iterator goToMove = everyGameObjectList.begin(); goToMove != everyGameObjectList.end(); goToMove++)
	{
		if ((*goToMove)->isMoved)
		{
			(*goToMove)->UpdateLocalTransform();
			(*goToMove)->UpdateGlobalTransform();

			(*goToMove)->isMoved = false;
			needToGenBuffers = true;
		}
	}
	if (needToGenBuffers)
		App->renderer3D->GenerateBuffers();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::Draw()
{
	p.Render();
}

GameObject* ModuleSceneIntro::AddGameObject(char* name)
{
	GameObject* ret;

	ret = new GameObject(name);

	rootNode->children.push_back(ret);
	ret->parent = rootNode;
	everyGameObjectList.push_back(ret);

	return ret;
}

