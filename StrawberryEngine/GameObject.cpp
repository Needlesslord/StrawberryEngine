#include "Globals.h"
#include "GameObject.h"
#include "ModuleAssetImporter.h"
#include "ModuleSceneIntro.h"
GameObject::GameObject(char* name)
{
	this->name = name;
	selected = false;
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
	this->goMesh = m;
}

void GameObject::AddChild(Mesh* m)
{
	this->childrenMeshes.push_back(m);
}

vec3 GameObject::GetCenter()
{
	vec3 ret;
	return ret;
}