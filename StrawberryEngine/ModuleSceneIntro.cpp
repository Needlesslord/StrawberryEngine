#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModuleImporter.h"
#include "MeshImporter.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "GameObject.h"
#include "ModuleInput.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
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

	house = App->importer->meshImporter->Load("Assets/BakerHouse.fbx");
	house->ChangeName("Baker house");
	
	for (std::list<Mesh*>::iterator meshIterator = house->childrenMeshes.begin(); meshIterator != house->childrenMeshes.end(); meshIterator++)
	{
		(*meshIterator)->textureNumber = 1;
	}
	//App->importer->Load("Assets/warrior.fbx");

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
	meshesSelected.clear();
	for (std::list<Mesh*>::iterator meshesToSelect = meshesList.begin(); meshesToSelect != meshesList.end(); meshesToSelect++)
	{
		if ((*meshesToSelect)->isSelected)
		{
			meshesSelected.push_back(*meshesToSelect);
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
	for (std::list<Mesh*>::iterator meshesToMove = meshesList.begin(); meshesToMove != meshesList.end(); meshesToMove++)
	{
		if ((*meshesToMove)->isMoved)
		{
			vec3 diff = (*meshesToMove)->position - (*meshesToMove)->previousPosition;
			for (int i = 0; i < (*meshesToMove)->num_vertex; i++)
			{
				(*meshesToMove)->vertex[i].x += diff.x;
				(*meshesToMove)->vertex[i].y += diff.y;
				(*meshesToMove)->vertex[i].z += diff.z;
			}
			(*meshesToMove)->previousPosition = (*meshesToMove)->position;
			(*meshesToMove)->isMoved = false;
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

GameObject* ModuleSceneIntro::AddGameObject(char* n)
{
	GameObject* ret;

	ret = new GameObject(n);

	gameObjectList.push_back(ret);

	return ret;
}