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
		AddDefaultName();
	}
	else
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
	this->meshComponent = m;
}

void GameObject::AddChild(GameObject* go)
{
	this->children.push_back(go);
	go->parent = this;
}

void GameObject::AddDefaultName()
{
	char* charName = new char[25];
	std::string stringName = ("Default Game Object (" + std::to_string(App->importer->gameObjectNameIterator) + ")");
	strcpy(charName, stringName.c_str());
	ChangeName(charName);
	charName = nullptr;
	App->importer->gameObjectNameIterator++;
}

void GameObject::UpdateRotation()
{
	float3 diff = (rotation - previousRotation) * DEGTORAD;
	previousRotation = rotation;
	Quat rot = Quat::FromEulerXYZ(diff.x, diff.y, diff.z);
	rotationQuat = rotationQuat * rot;
}

void GameObject::UpdateLocalTransform()
{
	UpdateRotation();
	localTransform = math::float4x4::FromTRS(position, rotationQuat, scale);	

	if (children.size() > 0)
	{
		for (std::list<GameObject*>::iterator childrenIterator = children.begin(); childrenIterator != children.end(); childrenIterator++)
		{
			(*childrenIterator)->UpdateLocalTransform();
		}
	}
}

void GameObject::UpdateGlobalTransform()
{
	isMoved = false;
	if (parent != App->scene_intro->rootNode)
	{
		globalTransform = parent->globalTransform * localTransform;
	}	
	else
	{
		globalTransform = localTransform;
	}

	for (std::list<GameObject*>::iterator goIterator = children.begin(); goIterator != children.end(); goIterator++)
	{
		(*goIterator)->UpdateGlobalTransform();
	}
}

void GameObject::UpdateAABB()
{
	if (meshComponent)
	{
		obb = meshComponent->localAABB;
		obb.Transform(globalTransform);
		// Generate global AABB
		aabb.SetNegativeInfinity();
		aabb.Enclose(obb);

		float a = aabb.maxPoint.y;
		std::string b = std::to_string(a);
		LOG(b.c_str());
	}

	for (std::list<GameObject*>::iterator goIterator = children.begin(); goIterator != children.end(); goIterator++)
	{
		(*goIterator)->UpdateAABB();
	}
}