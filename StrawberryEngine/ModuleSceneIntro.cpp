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

	if (App->input->GetKey(SDL_SCANCODE_DELETE) == KEY_DOWN)
	{
		for (std::list<GameObject*>::iterator goIterator = gameObjectSelected.begin(); goIterator != gameObjectSelected.end(); goIterator++)
		{
			if (!(*goIterator)->children.empty())
			{
				AddChildrenToDeathRow(*goIterator);
			}
			else
			{
				gameObjectsToDelete.push_back(*goIterator);
			}
		}
	}

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

update_status ModuleSceneIntro::PostUpdate(float dt)
{

	if (!gameObjectsToDelete.empty())
	{
		for (std::list<GameObject*>::iterator goToDelete = gameObjectsToDelete.begin(); goToDelete != gameObjectsToDelete.end(); goToDelete++)
		{
			DeleteGameObject(*goToDelete);
		}
		gameObjectsToDelete.clear();
	}

	if (!gameObjectsToReparent.empty())
	{
		for (std::list<GameObject*>::iterator goToReparent = gameObjectsToReparent.begin(); goToReparent != gameObjectsToReparent.end(); goToReparent++)
		{
			ReparentUp(*goToReparent);
		}
		gameObjectsToReparent.clear();
	}

	


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

GameObject* ModuleSceneIntro::AddEmptyGameObject(GameObject* parent, char* name)
{
	GameObject* ret;

	ret = new GameObject(name);

	if (parent != nullptr)
	{
		parent->AddChild(ret);
		ret->parent = parent;
	}
	
	everyGameObjectList.push_back(ret);

	return ret;
}

void ModuleSceneIntro::DeleteGameObject(GameObject* go)
{
	if (go->meshComponent != nullptr)
	{
		delete(go->meshComponent);
		go->meshComponent = nullptr;
	}
	
	if (go->isSelected)
	{
		gameObjectSelected.remove(go);
	}

	everyGameObjectList.remove(go);
	if (go->parent != nullptr)
	{
		go->parent->children.remove(go);
	}

	delete(go);
	go = nullptr;
}

void ModuleSceneIntro::AddChildrenToDeathRow(GameObject* go)
{
	for (std::list<GameObject*>::iterator goIterator = go->children.begin(); goIterator != go->children.end(); goIterator++)
	{
		AddChildrenToDeathRow(*goIterator);
	}
	gameObjectsToDelete.push_back(go);
}

void ModuleSceneIntro::ReparentUp(GameObject* go)
{
	if (go->parent != rootNode)
	{
		go->parent->children.remove(go);
		go->parent->parent->children.push_back(go);
		go->parent = go->parent->parent;
	}
}