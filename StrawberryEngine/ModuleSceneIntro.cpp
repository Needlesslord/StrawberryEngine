#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"

#include "ModuleImporter.h"
#include "MeshImporter.h"
#include "TextureImporter.h"
#include "GameObject.h"
#include "ModuleInput.h"
#include "ComponentCamera.h"

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

	camera01 = AddGameObject("Camera01");
	camera01->cameraComponent = (ComponentCamera*)camera01->AddComponent(Component::TYPE_CAMERA);
	App->camera->SetActiveCamera(camera01->cameraComponent);

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
			(*goToMove)->Update();

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

	if (!meshComponentsToDelete.empty())
	{
		for (std::list<GameObject*>::iterator meshToDelete = meshComponentsToDelete.begin(); meshToDelete != meshComponentsToDelete.end(); meshToDelete++)
		{
			RELEASE((*meshToDelete)->meshComponent);
		}
		meshComponentsToDelete.clear();
	}

	if (!textureComponentsToDelete.empty())
	{
		for (std::list<GameObject*>::iterator textureToDelete = textureComponentsToDelete.begin(); textureToDelete != textureComponentsToDelete.end(); textureToDelete++)
		{
			RELEASE((*textureToDelete)->textureComponent);
		}
		textureComponentsToDelete.clear();
	}

	if (App->camera->activeCamera != nullptr)
	{
		for (std::list<GameObject*>::iterator goToCull = everyGameObjectList.begin(); goToCull != everyGameObjectList.end(); goToCull++)
		{
			if ((*goToCull)->isActive && (*goToCull)->meshComponent)
			{
				(*goToCull)->isCulled = App->camera->activeCamera->NeedsCulling((*goToCull)->aabb);
			}
		}
	}

	return UPDATE_CONTINUE;
}


void ModuleSceneIntro::Draw()
{
	glLineWidth(1.0f);

	glBegin(GL_LINES);

	float d = 200.0f;

	for (float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}

	glEnd();
}

GameObject* ModuleSceneIntro::AddGameObject(char* name)
{
	GameObject* ret;

	ret = new GameObject(name);

	rootNode->children.push_back(ret);
	ret->parent = rootNode;
	everyGameObjectList.push_back(ret);

	ret->Update();

	return ret;
}

GameObject* ModuleSceneIntro::AddEmptyGameObject(GameObject* parent)
{
	GameObject* ret;

	ret = new GameObject();

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