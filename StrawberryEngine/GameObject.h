#include "Libs/MathGeoLib/include/MathBuildConfig.h"
#include "Libs/MathGeoLib/include/MathGeoLib.h"

class Mesh;

class GameObject
{

public:

	GameObject(char* name = nullptr);
	~GameObject();

	void AddDefaultName();
	void ChangeName(char* newName);
	void AddMesh(Mesh* m);
	void AddChild(GameObject* go);
	void UpdateTransform();

public:

	float3 position = { 0,0,0 };		// 
	float3 scale = { 0,0,0 };			// they have to be float 3 or we can't use them with matrices :/
	float3 rotation = { 0,0,0 };		// 
	Quat rotationQuat = Quat::identity;

	float4x4 transform = float4x4::identity;

	bool isMoved = false; // this should change to transformUpdated or smthg
	float3 previousPosition = { 0,0,0 }; // same to a prev. transform
	float3 previousRotation = { 0,0,0 };
	char* name;

	Mesh* meshComponent = nullptr; 

	std::list<GameObject*> children;

	bool isVertexNormalsEnabled = false;

	bool isSelected = false;

};
