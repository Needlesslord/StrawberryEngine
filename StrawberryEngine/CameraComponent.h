#include "Libs/MathGeoLib/include/MathGeoLib.h"
class CameraComponent
{
public:
	Frustum frustrum;

	char* name = nullptr;
	
	bool isActive = true;
};