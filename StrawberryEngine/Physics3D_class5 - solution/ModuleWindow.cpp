#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleUI.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	window = NULL;
	screen_surface = NULL;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		int width = SCREEN_WIDTH * SCREEN_SIZE;
		int height = SCREEN_HEIGHT * SCREEN_SIZE;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		//Use OpenGL 3.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(WIN_FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(WIN_RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(WIN_BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(WIN_FULLSCREEN_DESKTOP == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	SDL_SetWindowMinimumSize(window, 700, 500);
	
	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

void ModuleWindow::SetFullscreen(bool *fullscreen)
{
	if (*fullscreen)
	{
		App->ui->isWinFullscreenDesktop = false;
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	}

	else if (!*fullscreen)
		SDL_SetWindowFullscreen(window, !SDL_WINDOW_FULLSCREEN);
}

void ModuleWindow::SetResizable(bool* resizable)
{
	SDL_bool sdlResizable;

	if (*resizable)
	{
		sdlResizable = SDL_TRUE;
		SDL_SetWindowResizable(window, sdlResizable);
	}
	else if (!*resizable) 
	{
		sdlResizable = SDL_FALSE;
		SDL_SetWindowResizable(window, sdlResizable);
	}
}

void ModuleWindow::SetBorderless(bool* borderless)
{
	SDL_bool sdlBorderless;

	if (*borderless)
	{
		sdlBorderless = SDL_FALSE;	// REMEMBER: The function says bordeRED not borderLESS
		SDL_SetWindowBordered(window, sdlBorderless);
	}
	else if (!*borderless)
	{
		sdlBorderless = SDL_TRUE;	// REMEMBER: The function says bordeRED not borderLESS
		SDL_SetWindowBordered(window, sdlBorderless);
	}
}

void ModuleWindow::SetFullDesktop(bool* fullDesktop)
{
	if (*fullDesktop)
	{
		App->ui->isWinFullscreen = false;
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	else if (!*fullDesktop)
		SDL_SetWindowFullscreen(window, !SDL_WINDOW_FULLSCREEN_DESKTOP);
}

void ModuleWindow::ResizeScreen() 
{
	SDL_SetWindowSize(window, screen_surface->w, screen_surface->h);
}

void ModuleWindow::SetBrightness(float* brightness)
{
	SDL_SetWindowBrightness(window, *brightness);
}