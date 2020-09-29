#include "PhysVehicle3D.h"
#include "Primitive.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return 0.0f;
}