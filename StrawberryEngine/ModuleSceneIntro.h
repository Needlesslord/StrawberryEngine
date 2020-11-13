#pragma once
#include "Module.h"
#include "Primitive.h"

class Mesh;
class GameObject;
class Texture;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

	void Draw();

	GameObject* AddGameObject(char* n);

public:
	
	Plane p;

	std::list<Mesh*> meshesList;
	std::list<Mesh*> meshesSelected;
	std::list<GameObject*> gameObjectList;

	std::list<Texture*> textureList;

	GameObject* house;

};