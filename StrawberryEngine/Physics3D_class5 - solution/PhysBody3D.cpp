#include "PhysBody3D.h"
#include "glmath.h"

// =================================================
PhysBody3D::PhysBody3D(btRigidBody* body) : body(body)
{}

// ---------------------------------------------------------
PhysBody3D::~PhysBody3D()
{
	delete body;
}

// ---------------------------------------------------------
void PhysBody3D::Push(float x, float y, float z)
{
}

// ---------------------------------------------------------
void PhysBody3D::GetTransform(float* matrix) const
{
	if(body != NULL && matrix != NULL)
	{
	}
}

// ---------------------------------------------------------
void PhysBody3D::SetTransform(const float* matrix) const
{
}

// ---------------------------------------------------------
void PhysBody3D::SetPos(float x, float y, float z)
{
}
