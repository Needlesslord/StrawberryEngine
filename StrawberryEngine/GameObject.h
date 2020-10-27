class Mesh;


class GameObject
{

public:

	GameObject(char* name = GAME_OBJECT_DEFAULT_NAME);
	~GameObject();

	void ChangeName(char* newName);
	void AddMesh(Mesh* m);
	void AddChild(Mesh* m);

public:

	char* name;
	Mesh* goMesh; // Game Object Mesh
	std::list<Mesh*> childrenMeshes;

};
