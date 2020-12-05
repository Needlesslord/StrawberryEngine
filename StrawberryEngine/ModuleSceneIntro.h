#pragma once
#include "Module.h"

class GameObject;
class Texture;
class Mesh;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();

	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);

	bool CleanUp();

	void Draw();

	GameObject* AddGameObject(char* name = nullptr);
	GameObject* AddEmptyGameObject(GameObject* parent = nullptr, char* name = nullptr);
	void DeleteGameObject(GameObject* go);
	void AddChildrenToDeathRow(GameObject* go);
	void ReparentUp(GameObject* go);

public:

	GameObject* rootNode = nullptr;
	//std::list<GameObject*> gameObjectList; // main game objects, not counting their children  --->> moveing this  to the RootNode's children' list
	std::list<GameObject*> everyGameObjectList; // all game objects
	std::list<GameObject*> gameObjectSelected;
	std::list<GameObject*> gameObjectsToDelete;
	std::list<GameObject*> gameObjectsToReparent;
	std::list<GameObject*> meshComponentsToDelete;
	std::list<GameObject*> textureComponentsToDelete;

	std::list<Mesh*> meshesList;

	std::list<Texture*> textureList;

	GameObject* street = nullptr;
	GameObject* house = nullptr;

	GameObject* camera01 = nullptr;
};