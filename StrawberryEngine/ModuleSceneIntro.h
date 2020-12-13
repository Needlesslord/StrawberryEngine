#pragma once
#include "Module.h"
#include "Libs/ImGuizmo/ImGuizmo.h"

class GameObject;
class Texture;
class Mesh;
class ComponentCamera;

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
	GameObject* AddEmptyGameObject(GameObject* parent = nullptr);
	void DeleteGameObject(GameObject* go);
	void AddChildrenToDeathRow(GameObject* go);
	void ReparentUp(GameObject* go);
	void DrawGuizmo();

public:

	GameObject* rootNode = nullptr;
	GameObject* gameObjectSelected = nullptr;

	//std::list<GameObject*> gameObjectList; // main game objects, not counting their children  --->> moveing this  to the RootNode's children' list
	std::list<GameObject*> everyGameObjectList; // all game objects
	std::list<GameObject*> gameObjectsToDelete;							//
	std::list<GameObject*> gameObjectsToReparent;						//
	std::list<GameObject*> meshComponentsToDelete;						// All these lists are to modify things on post update so the current frame ends normally
	std::list<GameObject*> textureComponentsToDelete;					//
	std::list<GameObject*> cameraComponentsToDelete;					//
	std::list<Texture*> texturesToDelete;								//

	std::list<Mesh*> meshesList;
	std::list<Texture*> textureList;
	std::list<ComponentCamera*> cameras; // All camera component (not the editor camera though)


	GameObject* street = nullptr;
	GameObject* house = nullptr;

	GameObject* camera01 = nullptr;

	ImGuizmo::OPERATION currentOperation = ImGuizmo::OPERATION::TRANSLATE; // default
	ImGuizmo::MODE currentMode = ImGuizmo::MODE::WORLD;
};