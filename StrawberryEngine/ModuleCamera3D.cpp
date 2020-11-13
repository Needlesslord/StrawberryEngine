#include "Globals.h"
#include "Application.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"
#include "MeshImporter.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 8.5f);
	Reference = vec3(0.0f, 0.0f, 0.0f);
}

ModuleCamera3D::~ModuleCamera3D()
{}

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

	vec3 newPos(0,0,0);
	float speed = 3.0f * dt;





		// Holding SHIFT duplicates movement speed.
	if(App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed *= 2;






		// While Right clicking, “WASD” fps-like movement must be enabled.
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos.y += speed; 
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos.y -= speed; 

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;
	}





		// Mouse wheel should zoom in and out
	float DeltaZ = App->input->GetMouseZ();
	if (DeltaZ != 0)
	{
		newPos -= Z * DeltaZ;
	}

	Position += newPos;
	Reference += newPos;

	


		// While Right clicking, free look around must be enabled.
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.10f;

		vec3 ref = Reference;

		//Position -= Reference;

		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			//X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			//Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			//Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));

			ref.x = Reference.x - DeltaX;
		}

		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			//Y = rotate(Y, DeltaY, X);
			//Z = rotate(Z, DeltaY, X);

			if (Y.y < 0.0f)
			{
				//Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				//Y = cross(Z, X);
			}

			if (isYInverted) {
				ref.y = Reference.y - DeltaY;
			}
			else
			{
				ref.y = Reference.y + DeltaY;
			}
		}

		ref.z = Reference.z;

		//Position = Reference + Z * length(Position);
		LookAt(ref);
	}

	

	
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

update_status ModuleCamera3D::PostUpdate(float dt)
{


		// Alt+Left click should orbit the object.
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && !App->scene_intro->meshesSelected.empty())
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.25f;

		Mesh* ref = (*App->scene_intro->meshesSelected.begin());
		Reference = ref->position;

		Position -= Reference;

		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}

		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);

			if (Y.y < 0.0f)
			{
				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = cross(Z, X);
			}
		}

		
		Position = Reference + Z * length(Position);
	}




		// Pressing “f” should focus the camera around the geometry.
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN && !App->scene_intro->meshesSelected.empty())
	{
		LookAt((*App->scene_intro->meshesSelected.begin())->position);
	}




	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}