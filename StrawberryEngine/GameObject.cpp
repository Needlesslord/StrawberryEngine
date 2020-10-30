#include "Globals.h"
#include "GameObject.h"
#include "ModuleImporter.h"
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

	vec3 biggest = { -1,-1,-1 };
	vec3 smallest = { 1,1,1 };

	for (std::list<Mesh*>::iterator meshIterator = this->childrenMeshes.begin(); meshIterator != this->childrenMeshes.end(); meshIterator++)
	{
		for (int i = 0; i < (*meshIterator)->num_vertex / 3; i += 3)
		{
			if ((*meshIterator)->vertex[i] > biggest.x)
			{
				biggest.x = (*meshIterator)->vertex[i];
			}
			else if ((*meshIterator)->vertex[i] < smallest.x)
			{
				smallest.x = (*meshIterator)->vertex[i];
			}

			if ((*meshIterator)->vertex[i + 1] > biggest.x)
			{
				biggest.x = (*meshIterator)->vertex[i + 1];
			}
			else if ((*meshIterator)->vertex[i + 1] < smallest.x)
			{
				smallest.x = (*meshIterator)->vertex[i + 1];
			}

			if ((*meshIterator)->vertex[i + 2] > biggest.x)
			{
				biggest.x = (*meshIterator)->vertex[i + 2];
			}
			else if ((*meshIterator)->vertex[i + 2] < smallest.x)
			{
				smallest.x = (*meshIterator)->vertex[i + 2];
			}
		}
	}

	ret.x = (biggest.x + smallest.x) / 2;
	ret.y = (biggest.y + smallest.y) / 2;
	ret.z = (biggest.z + smallest.z) / 2;

	return ret;
}