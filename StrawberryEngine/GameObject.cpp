#include "Globals.h"
#include "GameObject.h"
#include "MeshImporter.h"
#include "ModuleSceneIntro.h"
#include "Application.h"
#include "ModuleImporter.h"



GameObject::GameObject(char* name)
{
	if (name == nullptr)
	{
		AddDefName();
	}
	else
		this->name = name;

	meshComponent = new Mesh();
}

GameObject::~GameObject()
{
	
}

void GameObject::ChangeName(char* newName)
{
	this->name = newName;
}

void GameObject::AddMesh(Mesh* m)
{
	this->meshComponent = m;
}

void GameObject::AddChild(GameObject* go)
{
	this->children.push_back(go);
}

vec3 GameObject::GetCenter()
{
	vec3 ret;
	return ret;
}

void GameObject::AddDefName()
{
	char* charName = new char[25];
	std::string stringName = ("Default Game Object (" + std::to_string(App->importer->gameObjectNameIterator) + ")");
	strcpy(charName, stringName.c_str());
	ChangeName(charName);
	charName = nullptr;
	App->importer->gameObjectNameIterator++;
}