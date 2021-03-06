#include "Application.h"
#include "Globals.h"
#include "ModuleRenderer3D.h"

#include "ModuleUI.h"
#include "ModuleWindow.h"
#include "ModuleSceneIntro.h"
#include "ModuleImporter.h"
#include "TextureImporter.h"
#include "MeshImporter.h"
#include "ComponentCamera.h"
#include "GameObject.h"

#include "Libs/Glew/include/GL/glew.h"
#include "Libs/SDL/include/SDL_opengl.h"

#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	//App->ui->pendingOutputs.push_back("Creating 3D Renderer context"); // CACA
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	if(ret == true)
	{
		//LOG GL Properties
		LOG("Vendor: %s", glGetString(GL_VENDOR));
		LOG("Renderer: %s", glGetString(GL_RENDERER));
		LOG("OpenGL version supported %s", glGetString(GL_VERSION));
		LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));


		//Use Vsync
		if (VSYNC && SDL_GL_SetSwapInterval(1) < 0)
		{
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
		}

		//Initialize Glew
		GLenum err = glewInit();
		LOG("Using Glew %s", glewGetString(GLEW_VERSION));


		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));

			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		lights[0].Active(true);

		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);
	}

	OnResize(App->window->screen_surface->w, App->window->screen_surface->h);


	App->importer->textureImporter->defaultTexture = CreateCheckersTexture();


	return ret;
}

bool ModuleRenderer3D::Start()
{

	GenerateBuffers();
	


	return true;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	if (camera->isUpdateMatrix)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glLoadMatrixf((GLfloat*)camera->GetProjectionMatrix());

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		camera->isUpdateMatrix = false;
	}

	background = Black;
	glClearColor(background.r, background.g, background.b, background.a);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf((GLfloat*)camera->GetViewMatrix());
	

	// light 0 on cam pos
	lights[0].SetPos(camera->frustum.pos.x,camera->frustum.pos.y, camera->frustum.pos.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	
	//glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	
	if (App->ui->isGridEnabled)
	{
		App->scene_intro->Draw();
	}

	for (std::list<ComponentCamera*>::iterator cameraIterator = App->scene_intro->cameras.begin(); cameraIterator != App->scene_intro->cameras.end(); cameraIterator++)
	{
		if ((*cameraIterator)->isDebugEnabled)
		{
			DrawCameraFrustum((*cameraIterator));
		}
	}

	if (App->ui->isDrawEnabled)
	{
		Draw(App->scene_intro->rootNode);
	}

	

	// Not working yet
	//App->ui->DrawFrame(texColorBuffer);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if (App->scene_intro->gameObjectSelected != nullptr)
	{
		App->scene_intro->DrawGuizmo();
	}

	App->ui->Draw();
	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	glDeleteFramebuffers(1, &frameBuffer);
	glDeleteRenderbuffers(1, &rboDepthStencil);

	return true;
}

void ModuleRenderer3D::OnResize(int width, int height)
{
	App->window->screen_surface->w = width;
	App->window->screen_surface->h = height;

	App->ui->ResetWindowInits();

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glLoadMatrixf((GLfloat*)camera->GetProjectionMatrix());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	// Frame Buffer setup
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, App->window->screen_surface->w, App->window->screen_surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

	glGenRenderbuffers(1, &rboDepthStencil);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepthStencil);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, App->window->screen_surface->w, App->window->screen_surface->h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepthStencil);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG("Error setting up frame buffer");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void ModuleRenderer3D::DrawCubeDirect()
{
	// vertexes
	float v0[] = { 1.f, 1.f, 1.f };
	float v1[] = { 0.f, 1.f, 1.f };
	float v2[] = { 0.f, 0.f, 1.f };
	float v3[] = { 1.f, 0.f, 1.f };
	float v4[] = { 1.f, 0.f, 0.f };
	float v5[] = { 1.f, 1.f, 0.f };
	float v6[] = { 0.f, 1.f, 0.f };
	float v7[] = { 0.f, 0.f, 0.f };

	glBegin(GL_TRIANGLES);  // draw a cube with 12 triangles

	// front face =================
	glVertex3fv(v0);    // v0-v1-v2
	glVertex3fv(v1);
	glVertex3fv(v2);

	glVertex3fv(v2);    // v2-v3-v0
	glVertex3fv(v3);
	glVertex3fv(v0);

	// right face =================
	glVertex3fv(v0);    // v0-v3-v4
	glVertex3fv(v3);
	glVertex3fv(v4);

	glVertex3fv(v4);    // v4-v5-v0
	glVertex3fv(v5);
	glVertex3fv(v0);

	// top face ===================
	glVertex3fv(v0);    // v0-v5-v6
	glVertex3fv(v5);
	glVertex3fv(v6);

	glVertex3fv(v6);    // v6-v1-v0
	glVertex3fv(v1);
	glVertex3fv(v0);

	// left face ==================
	glVertex3fv(v7);    
	glVertex3fv(v2);
	glVertex3fv(v1);

	glVertex3fv(v1);
	glVertex3fv(v6);
	glVertex3fv(v7);

	// back face ==================
	glVertex3fv(v7);
	glVertex3fv(v6);
	glVertex3fv(v5);

	glVertex3fv(v5);
	glVertex3fv(v4);
	glVertex3fv(v7);

	// bot face ===================
	glVertex3fv(v7);
	glVertex3fv(v4);
	glVertex3fv(v3);

	glVertex3fv(v3);
	glVertex3fv(v2);
	glVertex3fv(v7);

	glEnd();
}

void ModuleRenderer3D::DrawCubeArray()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glDrawArrays(GL_TRIANGLES, 0, 12);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ModuleRenderer3D::DrawCubeIndices()
{
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, myId);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myIndeces);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_VERTEX_ARRAY);
}

void ModuleRenderer3D::GenerateBuffers()
{
	for (std::list<Mesh*>::iterator meshIterator = App->scene_intro->meshesList.begin(); meshIterator != App->scene_intro->meshesList.end(); meshIterator++)
	{
		Mesh* mesh = (*meshIterator);
		glGenBuffers(1, (GLuint*) & (mesh->id_vertex));
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertex);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_vertex * 3, mesh->vertex, GL_STATIC_DRAW);

		glGenBuffers(1, (GLuint*) & (mesh->id_index));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_index);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->num_index, mesh->index, GL_STATIC_DRAW);
	}
}

void ModuleRenderer3D::Draw(GameObject* go)
{
	bool isChecked = true;
	if (go->meshComponent != nullptr)
	{
		if (App->camera->isCullingActive)
		{
			if (go->isCulled)
				isChecked = false;
		}

		if (go->meshComponent->isActive && isChecked)
		{

			glPushMatrix();
			glMultMatrixf((GLfloat*)&go->globalTransform.Transposed()); // If it's not transposed the translation goes weird... also, it should be global 


			Mesh* mesh = go->meshComponent->mesh;

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertex);
			glVertexPointer(3, GL_FLOAT, 0, NULL);


			// Draw texture
			if (App->ui->isTexturesEnabled && go->textureComponent != nullptr)
			{
				if (go->textureComponent->isActive)
				{
					glBindBuffer(GL_ARRAY_BUFFER, mesh->id_tex_coord);

					glBindTexture(GL_TEXTURE_2D, go->textureComponent->texture->GetId());
					glTexCoordPointer(2, GL_FLOAT, 0, NULL);
				}
			}


			// Draw normals
			if (go->isVertexNormalsEnabled && mesh->hasNormals)
			{
			
				for (int i = 0; i < mesh->num_vertex; i++)
				{
					float vX = mesh->vertex[i].x;
					float vY = mesh->vertex[i].y;
					float vZ = mesh->vertex[i].z;

					glBegin(GL_LINES);
					glColor3f(1, 0, 1);
					glVertex3f(vX, vY, vZ);
					glVertex3f(vX + mesh->normals[i].x, vY + mesh->normals[i].y, vZ + mesh->normals[i].z);
				}

				glEnd();
				glColor3f(1, 1, 1);
			}


		

			if (isToggleWireframe)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glColor3f(1, 0, 1);
			}

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_index);
			glDrawElements(GL_TRIANGLES, mesh->num_index, GL_UNSIGNED_INT, NULL);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			if (isToggleWireframe)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glColor3f(1, 1, 1);
			}

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);



			glPopMatrix();

			// Draw AABB
			if (go->isAABBEnabled)
			{

				glBegin(GL_LINES);
				glColor3f(1, 0, 1);

				for (uint i = 0; i < 12; i++)
				{
					float vX = go->aabb.Edge(i).a.x;
					float vY = go->aabb.Edge(i).a.y;
					float vZ = go->aabb.Edge(i).a.z;

					float v2X = go->aabb.Edge(i).b.x;
					float v2Y = go->aabb.Edge(i).b.y;
					float v2Z = go->aabb.Edge(i).b.z;

					glVertex3f(vX, vY, vZ);
					glVertex3f(v2X, v2Y, v2Z);
				}

				glColor3f(1, 1, 1);
				glEnd();
			}
		}
	}
	
	for (std::list<GameObject*>::iterator goIterator = go->children.begin(); goIterator != go->children.end(); goIterator++)
	{
		Draw(*goIterator);
	}

}

Texture* ModuleRenderer3D::CreateCheckersTexture()
{
	Texture* ret = new Texture();

	GLubyte checkerImage[32][32][4];
	for (int i = 0; i < 32; i++) {
		for (int j = 0; j < 32; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = (GLubyte)c;
			checkerImage[i][j][2] = (GLubyte)c;
			checkerImage[i][j][3] = (GLubyte)255;
		}
	}
	ret->w = 32;
	ret->h = 32;
	ret->id = 0;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &ret->id);
	glBindTexture(GL_TEXTURE_2D, ret->id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);
	glBindTexture(GL_TEXTURE_2D, 0);

	App->scene_intro->textureList.push_back(ret);

	ret->fileName = "Default Texture.png";
	ret->name = "Default Texture";

	char* buffer;
	App->importer->textureImporter->Save(ret, &buffer);

	

	return ret;
}

void ModuleRenderer3D::ToggleDepthTest(const bool switchTo)
{
	if (switchTo == true)
	{
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}
}

void ModuleRenderer3D::ToggleLighting(const bool switchTo)
{
	if (switchTo)
	{
		glEnable(GL_LIGHTING);
	}
	else
	{
		glDisable(GL_LIGHTING);
	}
}

void ModuleRenderer3D::ToggleBackFaceCull(const bool switchTo)
{
	if (switchTo)
	{
		glEnable(GL_CULL_FACE);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}
}

void ModuleRenderer3D::ToggleTextures(const bool switchTo)
{
	if (switchTo)
	{
		glEnable(GL_TEXTURE_2D);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}
}

void ModuleRenderer3D::ToggleWireframe(const bool switchTo)
{
	isToggleWireframe = switchTo;
	if (!switchTo)
	{
		glEnable(GL_TEXTURE_2D);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}
}

void ModuleRenderer3D::DrawCameraFrustum(ComponentCamera* cam)
{
	glLineWidth(2.0f); // Much better tbh
	glBegin(GL_LINES);

	float3 corners[8];
	cam->frustum.GetCornerPoints(corners);

	glColor3f(1, 0, 1);

	// Left Face
	glVertex3f(corners[0].x, corners[0].y, corners[0].z);
	glVertex3f(corners[1].x, corners[1].y, corners[1].z);

	glVertex3f(corners[2].x, corners[2].y, corners[2].z);
	glVertex3f(corners[3].x, corners[3].y, corners[3].z);

	// Right Face
	glVertex3f(corners[4].x, corners[4].y, corners[4].z);
	glVertex3f(corners[5].x, corners[5].y, corners[5].z);

	glVertex3f(corners[6].x, corners[6].y, corners[6].z);
	glVertex3f(corners[7].x, corners[7].y, corners[7].z);

	// Near Plane
	glVertex3f(corners[0].x, corners[0].y, corners[0].z);
	glVertex3f(corners[4].x, corners[4].y, corners[4].z);

	glVertex3f(corners[2].x, corners[2].y, corners[2].z);
	glVertex3f(corners[6].x, corners[6].y, corners[6].z);

	glVertex3f(corners[0].x, corners[0].y, corners[0].z);
	glVertex3f(corners[2].x, corners[2].y, corners[2].z);

	glVertex3f(corners[4].x, corners[4].y, corners[4].z);
	glVertex3f(corners[6].x, corners[6].y, corners[6].z);

	// Far Plane
	glVertex3f(corners[1].x, corners[1].y, corners[1].z);
	glVertex3f(corners[5].x, corners[5].y, corners[5].z);

	glVertex3f(corners[3].x, corners[3].y, corners[3].z);
	glVertex3f(corners[7].x, corners[7].y, corners[7].z);

	glVertex3f(corners[1].x, corners[1].y, corners[1].z);
	glVertex3f(corners[3].x, corners[3].y, corners[3].z);

	glVertex3f(corners[5].x, corners[5].y, corners[5].z);
	glVertex3f(corners[7].x, corners[7].y, corners[7].z);



	float3 a = cam->gameObject->position;

	glVertex3f(a.x, a.y, a.z - 0.1f);
	glVertex3f(a.x, a.y, a.z + 0.1f);
	glVertex3f(a.x, a.y + 0.1f, a.z);
	glVertex3f(a.x, a.y - 0.1f, a.z);
	glVertex3f(a.x + 0.1f, a.y, a.z);
	glVertex3f(a.x - 0.1f, a.y, a.z);

	glColor3f(1, 1, 1);
	glEnd();
}