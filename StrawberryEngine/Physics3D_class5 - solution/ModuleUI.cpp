#include "Globals.h"
#include "Application.h"
#include "ModuleUI.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"

#include <GL/glew.h>

#include "Libs/ImGui/imgui.h"
#include "Libs/ImGui/imgui_impl_sdl.h"
#include "Libs/ImGui/imgui_impl_opengl3.h"
#include "Libs/ImGui/imgui_internal.h"

ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
}

ModuleUI::~ModuleUI()
{

}

bool ModuleUI::Init()
{
	IMGUI_CHECKVERSION();

	return true;
}

bool ModuleUI::Start()
{
	bool ret = true;

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init("#version 130");

	
	return ret;
}

update_status ModuleUI::PreUpdate(float dt)
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
		if (event.type == SDL_QUIT)
			done = UPDATE_CONTINUE;
		if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(App->window->window))
			done = UPDATE_CONTINUE;
	}

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

update_status ModuleUI::Update(float dt)
{
	ImGui::BeginMainMenuBar();
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Quit"))
				App->input->quit = true;

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			ImGui::Checkbox("Demo", &show_demo_window);
			
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Documentation"))
				App->RequestBrowser("https://github.com/EnricGDV/Kore-Engine/wiki");

			ImGui::EndMenu();
		}
		/*
		if (ImGui::BeginMenu("Project"))
		{
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Build"))
		{
			ImGui::EndMenu();
		}
		*/
	}
	ImGui::EndMainMenuBar();

	if(show_demo_window)
		ImGui::ShowDemoWindow();

	return UPDATE_CONTINUE;
}

update_status ModuleUI::PostUpdate(float dt)
{
	
	return UPDATE_CONTINUE;
}

bool ModuleUI::CleanUp()
{
	LOG("Cleaning ui");

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

void ModuleUI::Draw() 
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}