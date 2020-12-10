#pragma once
#include "Module.h"
#include "Globals.h"
#include "Libs/MathGeoLib/include/Math/float3.h"

class ComponentCamera;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Init();
	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void Look(const float3& Position, const float3 &Reference, bool RotateAroundReference = false);
	void LookAt(const float3& spot);
	void Move(const float3& Movement);

	void SetActiveCamera(ComponentCamera* camera);

	void OnMouseClick(float2 mousePos);

private:

	void CalculateViewMatrix();

public:
	
	//float3 X, Y, Z, Position, Reference;
	float3 Reference;

	float cameraSpeed = 6.0f;
	float cameraSensitivity = 0.25f;

	bool isCullingActive = true;

	ComponentCamera* camera = nullptr;

	std::list<ComponentCamera*> cameras;

private:

};