class Mesh;

class GameObject
{

public:

	GameObject(char* name = GAME_OBJECT_DEFAULT_NAME);
	~GameObject();

	void ChangeName(char* newName);
	void AddMesh(Mesh* m);
	void AddChild(Mesh* m);
	vec3 GetCenter();

public:

	vec3 position = { 0,0,0 };
	vec3 rotation = { 0,0,0 };
	vec3 scale = { 0,0,0 };

	bool isMoved = false;

	char* name = nullptr;
	Mesh* goMesh = nullptr; // Game Object Mesh
	std::list<Mesh*> childrenMeshes;
	bool selected;

};
