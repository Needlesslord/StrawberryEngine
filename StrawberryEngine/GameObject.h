class Mesh;

#include "Libs/MathGeo/include/MathBuildConfig.h"
#include "Libs/MathGeo/include/MathGeoLib.h"

class GameObject
{

public:

	GameObject(char* name = nullptr);
	~GameObject();

	void AddDefName();
	void ChangeName(char* newName);
	void AddMesh(Mesh* m);
	void AddChild(GameObject* go);
	vec3 GetCenter();

public:

	vec3 position = { 0,0,0 };
	vec3 rotation = { 0,0,0 };
	//Quat rotationQuat(0.f, 0.f, 0.f, 1.f);
	vec3 scale = { 0,0,0 };

	bool isMoved = false;
	vec3 previousPosition = { 0,0,0 };

	char* name;

	Mesh* meshComponent = nullptr; 

	std::list<GameObject*> children;

	bool isVertexNormalsEnabled = false;

	bool isSelected = false;

};
