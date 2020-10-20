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

	name = TITLE;
	organization = "UPC CITM";
	
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
			ImGui::Checkbox("Demo", &isDemoShown);
			ImGui::Checkbox("Config", &isConfigShown);

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Documentation")) {}
				//App->RequestBrowser("https://github.com/");

			if (ImGui::MenuItem("Download latest")) {}
				//App->RequestBrowser("https://github.com/");

			if (ImGui::MenuItem("Report a bug")) {}
				//App->RequestBrowser("https://github.com/");

			if (ImGui::MenuItem("About"))
				isAboutShown = !isAboutShown;

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

	if(isDemoShown)
		ImGui::ShowDemoWindow();

	if (isConfigShown)
	{
		ImGui::Begin("Configuration");
		{
			if (ImGui::BeginMenu("Options"))
			{
				ImGui::EndMenu();
			}
			if (ImGui::CollapsingHeader("Application"))
			{
				ImGui::InputText("Engine Name", name, 32);
				ImGui::InputText("Organization", organization, 32);
				ImGui::SliderInt("Max FPS", &App->maxFps, 0, 200);
				ImGui::Text("Limit Framerate: %d", App->maxFps);
				//
				ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
				//

				char title[25];
				sprintf_s(title, 25, "Framerate %.1f", App->fps[App->fps.size() - 1]);
				ImGui::PlotHistogram("##framerate", &App->fps[0], App->fps.size(), 0, title, 0.0f, 200.0f, ImVec2(310, 100));
				sprintf_s(title, 25, "Milliseconds %.1f", App->ms[App->ms.size() - 1]);
				ImGui::PlotHistogram("##milliseconds", &App->ms[0], App->ms.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
			}
			if (ImGui::CollapsingHeader("Window"))
			{
				ImGui::Checkbox("Active", &isActive);
				ImGui::Text("Icon:    *default*");
				ImGui::SliderFloat("Brightness", &brightness, 1.0f, 100.0f);
				ImGui::SliderInt("Width", &App->window->screen_surface->w, 0.0f, 1920);
				ImGui::SliderInt("Height", &App->window->screen_surface->h, 0.0f, 1080);

				ImGui::Text("Refresh rate: 59"); // Idk what to put here

				if (ImGui::Checkbox("Fullscreen", &winFullscreen))
					App->window->SetFullscreen(&winFullscreen);
				ImGui::SameLine();
				ImGui::Checkbox("Resizable", &winResizable);
				ImGui::Checkbox("Borderless", &winBorderless);				
				ImGui::SameLine();
				ImGui::Checkbox("Full Desktop", &winFullscreenDesktop);
			}
		}
		ImGui::End();
	}

	if (isAboutShown) 
	{
		ImGui::Begin("About");
		{
			ImGui::Text("Strawberry Engine");
		}
		ImGui::End();
	}

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