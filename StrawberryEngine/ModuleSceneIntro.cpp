#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModuleImporter.h"
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

	house = App->importer->Load("Assets/BakerHouse.fbx");
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
	/*if (!meshesSelected.empty())
	{
		for (std::list<Mesh*>::iterator meshesToUnselect = meshesSelected.begin(); meshesToUnselect != meshesSelected.end(); meshesToUnselect++)
		{
			(*meshesToUnselect)->selected = false;
		}
		meshesSelected.clear();
		App->ui->isMeshSelected = false;
	}*/
	meshesSelected.clear();
	return UPDATE_CONTINUE;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	
	p = { 0.0, 1.0, 0.0, 0.0 };
	p.axis = true;

	for (std::list<Mesh*>::iterator meshesToSelect = meshesList.begin(); meshesToSelect != meshesList.end(); meshesToSelect++)
	{
		if ((*meshesToSelect)->selected)
		{
			meshesSelected.push_back(*meshesToSelect);
		}
	}


	if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN) // TESTING
	{
		for (std::list<Mesh*>::iterator meshesToMove = meshesList.begin(); meshesToMove != meshesList.end(); meshesToMove++)
		{
			for (int i = 2; i < (*meshesToMove)->num_vertex; i += 3)
			{
				LOG("Original position: %d", (*meshesToMove)->vertex[i]);
				(*meshesToMove)->vertex[i] += 1.f;
				LOG("Original position: %d", (*meshesToMove)->vertex[i]);
			}
		}
		App->renderer3D->GenerateBuffers();
	}

	

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