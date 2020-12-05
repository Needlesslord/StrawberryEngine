#ifndef __MESH_H__
#define __MESH_H__

#include "Globals.h"
#include "Libs/MathGeoLib/include/Geometry/AABB.h"

class Mesh
{
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

	AABB localAABB;

	std::string name;
	std::string fileName;

};
#endif //__MESH_H__