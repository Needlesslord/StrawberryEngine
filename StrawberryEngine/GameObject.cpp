#include "Globals.h"
#include "GameObject.h"
#include "ModuleImporter.h"

GameObject::GameObject(char* name)
{
	this->name = name;
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