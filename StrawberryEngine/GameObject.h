#include "Libs/MathGeoLib/include/MathBuildConfig.h"
#include "Libs/MathGeoLib/include/MathGeoLib.h"

class MeshComponent;
class TextureComponent;
class GameObject
{
public:
	GameObject();
	GameObject(std::string name);
	~GameObject();

	void AddDefaultName();
	void ChangeName(std::string newName = "");
	void AddMesh(MeshComponent* m);
	void AddChild(GameObject* go);
	void UpdateRotation();
	void UpdateLocalTransform();
	void UpdateGlobalTransform();
	void UpdateAABB();

public:

	float3 position = { 0,0,0 };		// 
	float3 scale = { 0,0,0 };			// they have to be float 3 or we can't use them with matrices :/
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

	MeshComponent* meshComponent = nullptr; 
	TextureComponent* textureComponent = nullptr;

	std::list<GameObject*> children;

	GameObject* parent = nullptr;

	bool isVertexNormalsEnabled = false;
	bool isAABBEnabled = false;

	bool isSelected = false;

};
