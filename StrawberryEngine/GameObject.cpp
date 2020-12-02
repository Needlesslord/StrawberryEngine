#include "Globals.h"
#include "GameObject.h"
#include "MeshImporter.h"
#include "ModuleSceneIntro.h"
#include "Application.h"
#include "ModuleImporter.h"

GameObject::GameObject()
{
	AddDefaultName();
}

GameObject::GameObject(std::string name)
{
	if (name == "")
	{
		AddDefaultName();
	}
	else
		this->name = name;
}

GameObject::~GameObject()
{
	
}

void GameObject::ChangeName(std::string newName)
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
	std::string name = ("Default Game Object (" + std::to_string(App->importer->gameObjectNameIterator) + ")");
	ChangeName(name);
	App->importer->gameObjectNameIterator++;
}

void GameObject::UpdateRotation()
{
	float3 diff = (rotation - previousRotation) * DEGTORAD;
	previousRotation = rotation;
	Quat rot = Quat::FromEulerXYZ(diff.x, diff.y, diff.z);
	rotationQuat = rotationQuat * rot;

	if (children.size() > 0)
	{
		for (std::list<GameObject*>::iterator childrenIterator = children.begin(); childrenIterator != children.end(); childrenIterator++)
		{
			(*childrenIterator)->UpdateRotation();
		}
	}
}

void GameObject::UpdateLocalTransform()
{
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
	}

	for (std::list<GameObject*>::iterator goIterator = children.begin(); goIterator != children.end(); goIterator++)
	{
		(*goIterator)->UpdateAABB();
	}
}