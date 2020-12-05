#ifndef __COMPONENT_MESH_H__
#define __COMPONENT_MESH_H__

#include "Component.h"
#include "Mesh.h"

class ComponentMesh : public Component
{
public:

	ComponentMesh(Type type, GameObject* go);
	~ComponentMesh();

public:

	Mesh* mesh = nullptr;

};

#endif //__COMPONENT_MESH_H__