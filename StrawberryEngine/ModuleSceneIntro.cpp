#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"

#include "ModuleImporter.h"
#include "MeshImporter.h"
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

	house = App->importer->meshImporter->LoadMesh("Assets/Meshes/BakerHouse.fbx");
	
	for (std::list<GameObject*>::iterator goIterator = house->children.begin(); goIterator != house->children.end(); goIterator++)
	{
		(*goIterator)->meshComponent->textureNumber = 1;
	}

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

			/*vec3 diff = (*goToMove)->position - (*goToMove)->previousPosition;
			for (int i = 0; i < (*goToMove)->meshComponent->num_vertex; i++)
			{
				(*goToMove)->meshComponent->vertex[i].x += diff.x;
				(*goToMove)->meshComponent->vertex[i].y += diff.y;
				(*goToMove)->meshComponent->vertex[i].z += diff.z;
			}
			(*goToMove)->previousPosition = (*goToMove)->position;*/



			float3 rot = ((*goToMove)->rotation - (*goToMove)->previousRotation) * DEGTORAD;
			(*goToMove)->previousRotation = (*goToMove)->rotation;
			Quat quaternion_rot = Quat::FromEulerXYZ(rot.x, rot.y, rot.z);
			(*goToMove)->rotationQuat = (*goToMove)->rotationQuat * quaternion_rot;

			(*goToMove)->UpdateTransform();
			(*goToMove)->isMoved = false;
			needToGenBuffers = true;
		}

		
	}
	if (needToGenBuffers)
		App->renderer3D->GenerateBuffers();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
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
	everyGameObjectList.push_back(ret);

	return ret;
}

