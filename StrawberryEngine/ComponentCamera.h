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
	ComponentCamera();
	~ComponentCamera();

	bool Update();

	float* GetViewMatrix();
	float* GetProjectionMatrix();

	bool NeedsCulling(AABB& aabb);

public:
	
	Frustum frustum;
	float ratio = 1.3f;

	bool isUpdateMatrix = true;

	bool isCullingActive = false; // Simply for debug purposes

	bool isDebugEnabled = false; // Frustum debug

	bool isTestView = false; // To swap into this camera's view

};
#endif //__COMPONENT_CAMERA_H__