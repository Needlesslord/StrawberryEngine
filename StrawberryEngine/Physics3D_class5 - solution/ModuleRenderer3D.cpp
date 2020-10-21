#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleUI.h"
#include "ModuleWindow.h"
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
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());


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
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);
	}

	// Projection matrix for
	OnResize(App->window->screen_surface->w, App->window->screen_surface->h);

	return ret;
}

bool ModuleRenderer3D::Start()
{
	float vertices[] = { 0.f, 0.f, 0.f,   1.f, 0.f, 0.f,   1.f, 0.f, 1.f,   0.f, 0.f, 1.f,   0.f, 1.f, 1.f,   1.f, 1.f, 1.f,   1.f, 1.f, 0.f,   0.f, 1.f, 0.f };


	/*float cubeArray[] = { 0.f, 0.f, 0.f,   1.f, 0.f, 0.f,   1.f, 0.f, 1.f,   0.f, 0.f, 1.f,   0.f, 0.f, 0.f,   0.f, 0.f, 1.f,   0.f, 1.f, 1.f,   0.f, 1.f, 0.f,
						  0.f, 0.f, 0.f,   0.f, 1.f, 0.f,   1.f, 1.f, 0.f,   1.f, 0.f, 0.f,   0.f, 0.f, 1.f,   1.f, 0.f, 1.f,   1.f, 1.f, 1.f,   0.f, 1.f, 1.f,
						  1.f, 0.f, 0.f,   1.f, 1.f, 0.f,   1.f, 1.f, 1.f,   1.f, 0.f, 1.f,   0.f, 1.f, 0.f,   0.f, 1.f, 1.f,   1.f, 1.f, 1.f,   1.f, 1.f, 0.f };*/

	float cubeArray[] = { 0.f, 0.f, 0.f,  };

						  
	my_id = 0;
	glGenBuffers(1, (GLuint*) & (my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, vertices, GL_STATIC_DRAW);



	return true;
}
// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	background = Black;
	glClearColor(background.r, background.g, background.b, background.a);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	/*
	// CUBE
	//1
	glBegin(GL_POLYGON);
	glColor3b(100, 100, 100);
	glVertex3f(0.f, 0.f, 0.f);
	glColor3b(100, 100, 100);
	glVertex3f(1.f, 0.f, 0.f);
	glColor3b(100, 100, 100);
	glVertex3f(1.f, 0.f, 1.f);
	glColor3b(100, 100, 100);
	glVertex3f(0.f, 0.f, 1.f);
	glEnd();

	//2
	glBegin(GL_POLYGON);
	glColor3b(100, 100, 100);
	glVertex3f(0.f, 0.f, 0.f);
	glColor3b(100, 100, 100);
	glVertex3f(0.f, 0.f, 1.f);
	glColor3b(100, 100, 100);
	glVertex3f(0.f, 1.f, 1.f);
	glColor3b(100, 100, 100);
	glVertex3f(0.f, 1.f, 0.f);
	glEnd();

	//3
	glBegin(GL_POLYGON);
	glColor3b(100, 100, 100);
	glVertex3f(0.f, 0.f, 0.f);
	glColor3b(100, 100, 100);
	glVertex3f(0.f, 1.f, 0.f);
	glColor3b(100, 100, 100);
	glVertex3f(1.f, 1.f, 0.f);
	glColor3b(100, 100, 100);
	glVertex3f(1.f, 0.f, 0.f);
	glEnd();

	//4
	glBegin(GL_POLYGON);
	glColor3b(100, 100, 100);
	glVertex3f(0.f, 0.f, 1.f);
	glColor3b(100, 100, 100);
	glVertex3f(1.f, 0.f, 1.f);
	glColor3b(100, 100, 100);
	glVertex3f(1.f, 1.f, 1.f);
	glColor3b(100, 100, 100);
	glVertex3f(0.f, 1.f, 1.f);
	glEnd();

	//5
	glBegin(GL_POLYGON);
	glColor3b(100, 100, 100);
	glVertex3f(1.f, 0.f, 0.f);
	glColor3b(100, 100, 100);
	glVertex3f(1.f, 1.f, 0.f);
	glColor3b(100, 100, 100);
	glVertex3f(1.f, 1.f, 1.f);
	glColor3b(100, 100, 100);
	glVertex3f(1.f, 0.f, 1.f);
	glEnd();

	//6
	glBegin(GL_POLYGON);
	glColor3b(100, 100, 100);
	glVertex3f(0.f, 1.f, 0.f);
	glColor3b(100, 100, 100);
	glVertex3f(0.f, 1.f, 1.f);
	glColor3b(100, 100, 100);
	glVertex3f(1.f, 1.f, 1.f);
	glColor3b(100, 100, 100);
	glVertex3f(1.f, 1.f, 0.f);
	glEnd();
	*/

	

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	App->scene_intro->Draw();



	//glEnableClientState(GL_VERTEX_ARRAY);
	//glBindBuffer(GL_ARRAY_BUFFER, my_id);
	//glVertexPointer(3, GL_FLOAT, 0, NULL);
	//// … bind and use other buffers
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	//glDisableClientState(GL_VERTEX_ARRAY);


	App->ui->Draw();

	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
