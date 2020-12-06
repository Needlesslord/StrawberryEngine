#include "Globals.h"
#include "ComponentCamera.h"
#include "GameObject.h"

ComponentCamera::ComponentCamera(Type type, GameObject* go) : Component(type, go)
{
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = gameObject->globalTransform.TranslatePart();
	frustum.front = gameObject->globalTransform.WorldZ();
	frustum.up = gameObject->globalTransform.WorldY();
	frustum.nearPlaneDistance = 5.0f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = DEGTORAD * (55.0f);
	
	frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov * 0.5f) * ratio);
}

ComponentCamera::~ComponentCamera()
{

}

bool ComponentCamera::Update()
{
	bool ret = true;

	frustum.pos = gameObject->globalTransform.TranslatePart();
	frustum.front = gameObject->globalTransform.WorldZ();
	frustum.up = gameObject->globalTransform.WorldY();

	return ret;
}

bool ComponentCamera::NeedsCulling(AABB& aabb)
{
	float3 vCorner[8];
	int iTotalIn = 0;
	aabb.GetCornerPoints(vCorner);
	math::Plane m_plane[6];
	this->frustum.GetPlanes(m_plane);

	for (int p = 0; p < 6; ++p) {
		int iInCount = 8;
		int iPtIn = 1;
		for (int i = 0; i < 8; ++i) {
			// test this point against the planes
			if (m_plane[p].IsOnPositiveSide(vCorner[i])) { //<-- “IsOnPositiveSide” from MathGeoLib
				iPtIn = 0;
				--iInCount;
			}
		}
		// were all the points outside of plane p?
		if (iInCount == 0)
			return true;
		// check if they were all on the right side of the plane
		iTotalIn += iPtIn;
	}
	// we must be partly in then otherwise
	return false;
}