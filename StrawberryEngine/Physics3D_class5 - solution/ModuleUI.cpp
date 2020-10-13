#include "Globals.h"
#include "Application.h"
#include "ModuleUI.h"

#include <GL/glew.h>

#include "Libs/ImGUI/imgui.h"
#include "Libs/ImGUI/imgui_impl_sdl.h"
#include "Libs/ImGUI/imgui_impl_opengl3.h"

ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
}

ModuleUI::~ModuleUI()
{

}

bool ModuleUI::Start()
{
	LOG("Setting up the ui");
	bool ret = true;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	//ImGuiIO& io = ImGui::GetIO(); (void)io;
	////io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	////io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	//// Setup Dear ImGui style
	////ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	//SDL_GLContext gl_context = SDL_GL_CreateContext(App->window->window);

	//// Setup Platform/Renderer bindings
	//ImGui_ImplSDL2_InitForOpenGL(App->window->window, gl_context);

	//const char* glsl_version = "#version 130";
	//ImGui_ImplOpenGL3_Init(glsl_version);

	
	
	return ret;
}

update_status ModuleUI::PreUpdate(float dt)
{
	/*SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
		if (event.type == SDL_QUIT)
			done = true;
		if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(App->window->window))
			done = true;
	}*/

	return UPDATE_CONTINUE;
}

update_status ModuleUI::Update(float dt)
{

	//ImGui_ImplOpenGL3_NewFrame();
	//ImGui_ImplSDL2_NewFrame(App->window->window);
	//ImGui::NewFrame();

	//// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	//if (show_demo_window)
	//	ImGui::ShowDemoWindow(&show_demo_window);

	//ImGui::Render();
	//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}

update_status ModuleUI::PostUpdate(float dt)
{
	// Rendering
	
	//SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

bool ModuleUI::CleanUp()
{
	LOG("Cleaning ui");

	//ImGui_ImplOpenGL3_Shutdown();
	//ImGui_ImplSDL2_Shutdown();
	//ImGui::DestroyContext();

	return true;
}