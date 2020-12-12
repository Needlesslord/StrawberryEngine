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

	//App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	//App->camera->LookAt(vec3(0, 0, 0));

	//house = App->importer->meshImporter->LoadMesh("Assets/Meshes/BakerHouse.fbx");
	street = App->importer->meshImporter->LoadMesh("Assets/Meshes/Street environment_V01.FBX");
	street->rotation = { -90,0,0 };
	street->isMoved = true;
	street->ChangeName("Street");

	camera01 = AddGameObject("Camera01");
	camera01->cameraComponent = (ComponentCamera*)camera01->AddComponent(Component::TYPE_CAMERA);
	
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
	// Culling
	if (App->renderer3D->camera != nullptr)
	{
		for (std::list<GameObject*>::iterator goToCull = everyGameObjectList.begin(); goToCull != everyGameObjectList.end(); goToCull++)
		{
			if ((*goToCull)->isActive && (*goToCull)->meshComponent)
			{
				(*goToCull)->isCulled = App->renderer3D->camera->NeedsCulling((*goToCull)->aabb);
			}
		}
	}

	// Debug culling
	for (std::list<ComponentCamera*>::iterator cameraIterator = cameras.begin(); cameraIterator != cameras.end(); cameraIterator++)
	{
		if ((*cameraIterator)->isCullingActive)
		{
			for (std::list<GameObject*>::iterator goToCull = everyGameObjectList.begin(); goToCull != everyGameObjectList.end(); goToCull++)
			{
				if ((*goToCull)->isActive && (*goToCull)->meshComponent)
				{
					(*goToCull)->isCulled = (*cameraIterator)->NeedsCulling((*goToCull)->aabb);
				}
			}
		}
	}

	// Make sure only one game object is selected at a time and that we have a pointer to it
	for (std::list<GameObject*>::iterator goIterator = everyGameObjectList.begin(); goIterator != everyGameObjectList.end(); goIterator++)
	{
		(*goIterator)->isSelected = false;
		if (gameObjectSelected != nullptr)
			gameObjectSelected->isSelected = true;
	}

	return UPDATE_CONTINUE;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{

	if (App->input->GetKey(SDL_SCANCODE_DELETE) == KEY_DOWN && gameObjectSelected != nullptr)
	{
		AddChildrenToDeathRow(gameObjectSelected);
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && App->input->GetMouseButton(SDL_BUTTON_RIGHT) != KEY_REPEAT)
	{
		currentOperation = ImGuizmo::OPERATION::TRANSLATE;
		currentMode = ImGuizmo::MODE::WORLD;
	}
	else if(App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && App->input->GetMouseButton(SDL_BUTTON_RIGHT) != KEY_REPEAT)
	{
		currentOperation = ImGuizmo::OPERATION::ROTATE;
		currentMode = ImGuizmo::MODE::WORLD;
	}
	else if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN && App->input->GetMouseButton(SDL_BUTTON_RIGHT) != KEY_REPEAT)
	{
		currentOperation = ImGuizmo::OPERATION::SCALE;
		currentMode = ImGuizmo::MODE::LOCAL;
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

	if (!cameraComponentsToDelete.empty())
	{
		for (std::list<GameObject*>::iterator cameraToDelete = cameraComponentsToDelete.begin(); cameraToDelete != cameraComponentsToDelete.end(); cameraToDelete++)
		{
			RELEASE((*cameraToDelete)->cameraComponent);
		}
		cameraComponentsToDelete.clear();
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
		gameObjectSelected = nullptr;
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

void ModuleSceneIntro::DrawGuizmo()
{
	// Draw guizmos axis
	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	
	float4x4 matrix;
	GameObject* go = gameObjectSelected;

	matrix = go->globalTransform.Transposed();
	float4x4 viewMatrix = App->renderer3D->camera->frustum.ViewMatrix();
	float4x4 projectionMatrix = App->renderer3D->camera->frustum.ProjectionMatrix();

	ImGuizmo::Manipulate(viewMatrix.Transposed().ptr(), projectionMatrix.Transposed().ptr(), currentOperation, currentMode, (float*)matrix.ptr());

	if (ImGuizmo::IsUsing() == true)
	{
		go->localTransform = go->parent->globalTransform.Inverted() * matrix.Transposed();
		go->UpdateGlobalTransform();
		go->UpdateAABB();
		go->localTransform.Decompose(go->position, go->rotationQuat, go->scale);
		go->rotation = go->rotationQuat.ToEulerXYZ();
		go->UpdateRotation();

		App->camera->isPickingRequired = false;
	}
	
}