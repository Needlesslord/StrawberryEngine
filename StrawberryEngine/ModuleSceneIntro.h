#pragma once
#include "Module.h"
#include "Primitive.h"

class Mesh;
class GameObject;
class Texture;
class Primitive;

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

	GameObject* AddGameObject(char* name = nullptr);

public:

	prim::Plane p;

	GameObject* rootNode = nullptr;
	//std::list<GameObject*> gameObjectList; // main game objects, not counting their children  --->> moveing this  to the RootNode's children' list
	std::list<GameObject*> everyGameObjectList; // all game objects
	std::list<GameObject*> gameObjectSelected;
	std::list<Mesh*> meshesList;

	std::list<Texture*> textureList;

	GameObject* street;

};