#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "MeshImporter.h"
#include "ComponentCamera.h"
#include "ModuleWindow.h"
#include "GameObject.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	camera = new ComponentCamera();
	camera->frustum.pos = { 0.0f, 5.0f, 8.5f };
	CalculateViewMatrix();

	//X = float3(1.0f, 0.0f, 0.0f);
	//Y = float3(0.0f, 1.0f, 0.0f);
	//Z = float3(0.0f, 0.0f, 1.0f);
	
	//Position = float3(0.0f, 0.0f, 8.5f);
	//Reference = float3(0.0f, 0.0f, 0.0f);
}

ModuleCamera3D::~ModuleCamera3D()
{}


// -----------------------------------------------------------------
bool ModuleCamera3D::Init()
{
	bool ret = true;
	
	App->renderer3D->camera = camera;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	// Data init
	float3 newPos(0,0,0);
	float speed = cameraSpeed * dt;
	int dx = -App->input->GetMouseXMotion();
	int dy = -App->input->GetMouseYMotion();
	float Sensitivity = cameraSensitivity * dt;
	float DeltaX = (float)dx * Sensitivity;
	float DeltaY = (float)dy * Sensitivity;

	float3 goTarget;
	if (!App->scene_intro->gameObjectSelected.empty())
	{
		GameObject* ref = (*App->scene_intro->gameObjectSelected.begin());
		goTarget.x = ref->aabb.CenterPoint().x;
		goTarget.y = ref->aabb.CenterPoint().y;
		goTarget.z = ref->aabb.CenterPoint().z;
	}


	// Mouse Picking
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && !App->ui->IsAnyWindowHovered())
	{
		vec2 pos = { (float)App->input->GetMouseX(), (float)App->input->GetMouseY() };
		OnMouseClick(pos);
	}






		// Holding SHIFT duplicates movement speed.
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
	{
		speed *= 2;
		Sensitivity *= 2;

		DeltaX = (float)dx * Sensitivity;
		DeltaY = (float)dy * Sensitivity;
	}




		// While Right clicking, “WASD” fps-like movement must be enabled.
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) camera->frustum.pos.y += speed; 
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) camera->frustum.pos.y -= speed;

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) camera->frustum.pos.x += speed;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) camera->frustum.pos.x -= speed;
	}





		// Mouse wheel should zoom in and out
	float DeltaZ = App->input->GetMouseZ();
	bool isHovered = App->ui->IsAnyWindowHovered();
	if (DeltaZ != 0 && !isHovered)
	{
		camera->frustum.pos.z += DeltaZ;
	}


	


		// While Right clicking, free look around must be enabled.
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{

		if (dx != 0)
		{
			Quat rot = Quat::RotateY(DeltaX);
			camera->frustum.front = rot.Mul(camera->frustum.front).Normalized();
			camera->frustum.up = rot.Mul(camera->frustum.up).Normalized();
		}

		if (dy != 0)
		{
			Quat rot = Quat::RotateAxisAngle(camera->frustum.WorldRight(), DeltaY);
			float3 Y = rot.Mul(camera->frustum.up).Normalized();

			if (Y.y > 0.0f)
			{
				camera->frustum.up = Y;
				camera->frustum.front = rot.Mul(camera->frustum.front).Normalized();
			}
		}
	}

	
	

	

	// Alt+Left click should orbit the object.
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && !App->scene_intro->gameObjectSelected.empty())
	{
		float3 length = camera->frustum.pos - goTarget;

		if (dx != 0)
		{
			Quat rot(camera->frustum.up, DeltaX);
			length = rot.Transform(length);
		}

		if (dy != 0)
		{
			Quat rot(camera->frustum.WorldRight(), DeltaY);
			length = rot.Transform(length);
		}

		camera->frustum.pos = length + goTarget;
		LookAt(goTarget);
	}




	// Pressing “f” should focus the camera around the geometry.
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN && !App->scene_intro->gameObjectSelected.empty())
	{
		LookAt(goTarget);
	}

	
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

update_status ModuleCamera3D::PostUpdate(float dt)
{
	

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference)
{
	//this->Position = Position;
	//this->Reference = Reference;

	//Z = normalize(Position - Reference);
	//X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	//Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		//this->Reference = this->Position;
		//this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt(const float3 &spot)
{
	float3 vector = spot - camera->frustum.pos;
	float3x3 mat = float3x3::LookAt(camera->frustum.front, vector.Normalized(), camera->frustum.up, float3::unitY);

	camera->frustum.front = mat.MulDir(camera->frustum.front).Normalized();
	camera->frustum.up = mat.MulDir(camera->frustum.up).Normalized();

	//CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const float3& Movement)
{
	//Position += Movement;
	//Reference += Movement;

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	//Quat qq = Quat::identity;
	//float3x4 vm = float3x4(camera->frustum.pos.x, camera->frustum.pos.y, camera->frustum.pos.z, 0.f, qq.x, qq.y, qq.z, qq.w, 1.0f, 1.0f, 1.0f, 0.0f);
	//camera->frustum.SetWorldMatrix(vm.Inverted());
}

void ModuleCamera3D::SetActiveCamera(ComponentCamera* camera)
{
	for (std::list<ComponentCamera*>::iterator cameraIterator = cameras.begin(); cameraIterator != cameras.end(); cameraIterator++)
	{
		(*cameraIterator)->isActive = false;
	}
	camera->isActive = true;
	App->renderer3D->camera = camera;
}

void ModuleCamera3D::OnMouseClick(vec2 mousePos)
{
	mousePos = { mousePos.x / (float)App->window->screen_surface->w * 2.f - 1.f, -(mousePos.y / (float)App->window->screen_surface->h * 2.f - 1.f) };
	LineSegment picking = App->renderer3D->camera->frustum.UnProjectLineSegment(mousePos.x, mousePos.y);
}