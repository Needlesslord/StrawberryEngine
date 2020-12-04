#include "Libs/MathGeoLib/include/Geometry/Frustum.h"
#include "Libs/MathGeoLib/include/Math/float3.h"
#include "Libs/MathGeoLib/include/Geometry/AABB.h"
#include "Libs/MathGeoLib/include/Geometry/Plane.h"

class CameraComponent
{
public:
	CameraComponent();
	~CameraComponent();
	bool NeedsCulling(AABB& aabb);

public:
	
	Frustum frustum;

	std::string name;
	
	bool isActive = true;

	bool isDebugEnabled = false;

};