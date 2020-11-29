#include "vector3.h"
#include "Libs/MathGeoLib/include/MathGeoLib.h"

class GameObject;
class Mesh
{
public:
	Mesh() {};
	~Mesh() {};
public:

	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = nullptr;
	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	vec3* vertex = nullptr;
	uint id_tex_coord = 0;
	float* tex_coord = nullptr;

	bool hasTex_coords = false;

	bool hasNormals = false;
	vec3* normals = nullptr;

	uint textureNumber = 999;

	bool isDrawEnabled = true;
	
	GameObject* parent = nullptr;

	const char* path = nullptr;

	char* name = nullptr;

	AABB localBoundingBox;
	//AABB globalBboundingBox;

};