#ifndef __COMPONENT_CAMERA_H__
#define __COMPONENT_CAMERA_H__

#include "Component.h"

#include "Libs/MathGeoLib/include/Geometry/Frustum.h"
#include "Libs/MathGeoLib/include/Math/float3.h"
#include "Libs/MathGeoLib/include/Geometry/AABB.h"
#include "Libs/MathGeoLib/include/Geometry/Plane.h"

class ComponentCamera : public Component
{
public:

	ComponentCamera(Type type, GameObject* go);
	~ComponentCamera();

	bool Update();

	bool NeedsCulling(AABB& aabb);

public:
	
	Frustum frustum;
	float ratio = 1.3f;

	//bool isCullingActive = false;
	bool isDebugEnabled = false;

};
#endif //__COMPONENT_CAMERA_H__