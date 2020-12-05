#include <list>

#include "Component.h"

#include "Libs/MathGeoLib/include/MathBuildConfig.h"
#include "Libs/MathGeoLib/include/MathGeoLib.h"

class ComponentTransform;
class ComponentMesh;
class ComponentTexture;
class ComponentCamera;

class GameObject
{
public:
	GameObject();
	GameObject(std::string name);
	~GameObject();

	void AddDefaultName();
	void ChangeName(std::string newName);
	Component* AddComponent(Component::Type type);
	void AddChild(GameObject* go);
	bool Update();
	void UpdateRotation();
	void UpdateLocalTransform();
	void UpdateGlobalTransform();
	void UpdateAABB();
	void SetActive(const bool state);

public:

	float3 position = { 0,0,0 };		// 
	float3 scale = { 1,1,1 };			// they have to be float 3 or we can't use them with matrices :/
	float3 rotation = { 0,0,0 };		// 
	Quat rotationQuat = Quat::identity;

	float4x4 localTransform = float4x4::identity; 
	float4x4 globalTransform = float4x4::identity; // The transform of a game object in the world

	OBB obb;
	AABB aabb;

	bool isMoved = false; // this should change to transformUpdated or smthg

	float3 previousRotation = { 0,0,0 };
	
	//char* name; // Had to change it cause of the input
	std::string name;

	ComponentMesh* meshComponent = nullptr; 
	ComponentTexture* textureComponent = nullptr;
	ComponentCamera* cameraComponent = nullptr;

	std::list<GameObject*> children;

	GameObject* parent = nullptr;

	bool isVertexNormalsEnabled = false;
	bool isAABBEnabled = false;

	bool isSelected = false;

	bool isActive = true;

	bool isCulled = false;

};
