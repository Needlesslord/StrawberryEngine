#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

#define MAX_SNAKE 2

struct PhysBody3D;
struct PhysMotor3D;
class Mesh;

class GameObject
{
	GameObject();
	~GameObject();

	char* name;
	Mesh* goMesh; // Game Object Mesh
	std::list<Mesh*> childrenMeshes;
};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

	void Draw();

public:
	
	Plane p;

	std::list<Mesh*> meshesList;

};
