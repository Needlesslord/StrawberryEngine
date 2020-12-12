#include "Globals.h"
#include "GameObject.h"
#include "MeshImporter.h"
#include "ModuleSceneIntro.h"
#include "Application.h"
#include "ModuleImporter.h"

#include "ComponentMesh.h"
#include "ComponentTexture.h"
#include "ComponentCamera.h"

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

Component* GameObject::AddComponent(Component::Type type)
{
	Component* component = nullptr;

	if (type == Component::TYPE_NONE)
	{
		return component;
	}
	else if (type == Component::TYPE_MESH)
	{
		component = new ComponentMesh(type, this);
		return component;
	}
	else if (type == Component::TYPE_TEXTURE)
	{
		component = new ComponentTexture(type, this);
		return component;
	}
	else if (type == Component::TYPE_CAMERA)
	{
		component = new ComponentCamera(type, this);
		App->scene_intro->cameras.push_back((ComponentCamera*)component);
		return component;
	}
}

bool GameObject::Update()
{
	bool ret = true;

	UpdateRotation();
	UpdateLocalTransform();
	UpdateGlobalTransform();
	UpdateAABB();

	return ret;
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

	if (cameraComponent)
	{
		cameraComponent->Update();
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
		obb = meshComponent->mesh->localAABB;
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

void GameObject::SetActive(const bool state)
{
	isActive = state;

	if (meshComponent)
	{
		meshComponent->isActive = state;
	}

	if (textureComponent)
	{
		textureComponent->isActive = state;
	}

	for (std::list<GameObject*>::iterator goIterator = children.begin(); goIterator != children.end(); goIterator++)
	{
		(*goIterator)->SetActive(state);
	}
}