#include "Globals.h"
#include "Application.h"
#include "ModuleUI.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleImporter.h"

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

	isWinFullscreen = WIN_FULLSCREEN;
	isWinResizable = WIN_RESIZABLE;
	isWinBorderless = WIN_BORDERLESS;
	isWinFullscreenDesktop = WIN_FULLSCREEN_DESKTOP;
	
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
			if(ImGui::Button("Config"))
			{
				isConfigShown = true;
			}
			ImGui::Checkbox("Hierarchy", &isHierarchyShown);

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
		ImGui::SetNextWindowPos({ 920, 20 });
		ImGui::SetNextWindowSize({ 350,600 });
		ImGui::Begin("Configuration", &isConfigShown);
		{

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
				if (ImGui::SliderFloat("Brightness", &brightness, 0.01f, 1.0f))
					App->window->SetBrightness(&brightness);
				ImGui::SliderInt("Width", &App->window->screen_surface->w, 700.0f, 1550);
				ImGui::SliderInt("Height", &App->window->screen_surface->h, 500.0f, 800);
				ImGui::Text("Refresh rate: 59"); // Idk what to put here
				if (ImGui::Checkbox("Fullscreen", &isWinFullscreen)) 
					App->window->SetFullscreen(&isWinFullscreen);
				ImGui::SameLine();
				if (ImGui::Checkbox("Resizable", &isWinResizable))
					App->window->SetResizable(&isWinResizable);
				if (ImGui::Checkbox("Borderless", &isWinBorderless))
					App->window->SetBorderless(&isWinBorderless);
				ImGui::SameLine();
				if (ImGui::Checkbox("Full Desktop", &isWinFullscreenDesktop))
					App->window->SetFullDesktop(&isWinFullscreenDesktop);
			}



			if (ImGui::CollapsingHeader("Hardware"))
			{
				ImGui::Text("SDL Version:"); ImGui::SameLine();
				ImGui::TextColored({ 1,0,1,1 }, "2.0.12");
				ImGui::Separator();
				ImGui::Text("CPU's:"); ImGui::SameLine();
				ImGui::TextColored({ 1,0,1,1 }, "%d (Cache: %dKb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());
				ImGui::Text("System RAM:"); ImGui::SameLine();
				ImGui::TextColored({ 1,0,1,1 }, "%dGb", SDL_GetSystemRAM()/1000);
				ImGui::Text("Caps:"); ImGui::SameLine();
				if (SDL_Has3DNow()) ImGui::TextColored({ 1,0,1,1 }, "3DNow,"); ImGui::SameLine();
				if (SDL_HasAVX())ImGui::TextColored({ 1,0,1,1 }, "AVX,"); ImGui::SameLine();
				if (SDL_HasAltiVec())ImGui::TextColored({ 1,0,1,1 }, "AltiVec,"); ImGui::SameLine();
				if (SDL_HasMMX())ImGui::TextColored({ 1,0,1,1 }, "MMX,"); ImGui::SameLine();
				if (SDL_HasRDTSC())ImGui::TextColored({ 1,0,1,1 }, "RDTSC,"); 
				if (SDL_HasSSE()) ImGui::TextColored({ 1,0,1,1 }, "SSE,"); ImGui::SameLine();
				if (SDL_HasSSE2())ImGui::TextColored({ 1,0,1,1 }, "SSE2,"); ImGui::SameLine();
				if (SDL_HasSSE3())ImGui::TextColored({ 1,0,1,1 }, "SSE3,"); ImGui::SameLine();
				if (SDL_HasSSE41())ImGui::TextColored({ 1,0,1,1 }, "SSE41,"); ImGui::SameLine();
				if (SDL_HasSSE42())ImGui::TextColored({ 1,0,1,1 }, "SSE42,");
				ImGui::Separator();
			}



			if (ImGui::CollapsingHeader("Textures"))
			{
				ImGui::Checkbox("Show textures", &App->renderer3D->isTexturesShown);
			}



		}
		ImGui::End();
	}



	if (isHierarchyShown)
	{
		ImGui::SetNextWindowPos({ 20, 20 });
		ImGui::SetNextWindowSize({ 300, 700 });
		ImGui::Begin("Hierarchy");

		if (App->scene_intro->meshesList.size() > 0) {

			int i = 0;
			for (std::list<Mesh*>::iterator meshIterator = App->scene_intro->meshesList.begin(); meshIterator != App->scene_intro->meshesList.end(); meshIterator++)
			{
				if ((*meshIterator)->name == "Default") {
					ImGui::Text("%s_%d", (*meshIterator)->name, i);
					i++;
				}
				else ImGui::Text("%s", (*meshIterator)->name);
			}
		}

		ImGui::End();
	}



	if (isAboutShown) 
	{
		ImGui::Begin("About");
		{
			ImGui::Text("Strawberry Engine");
			ImGui::Text("The sweetest 3D Engine");
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Text("By Tomas Carreras and Nuria Lamonja. Supervised by professor Marc Garrigo");
			ImGui::Spacing();
			ImGui::Spacing();
			if (ImGui::MenuItem("GitHub"))
				App->RequestBrowser("https://github.com/Needlesslord/StrawberryEngine");
			ImGui::Spacing();
			ImGui::Text("3rd Party Libraries used:");

			SDL_version version;
			SDL_VERSION(&version);
			ImGui::BulletText("SDL %d.%d.%d", version.major, version.minor, version.patch);
			//ImGui::BulletText("SDL Mixer %d.%d.%d", version.major, version.minor, version.patch);

			ImGui::BulletText("Glew %s.%s.%s", glewGetString(GLEW_VERSION_MAJOR), glewGetString(GLEW_VERSION_MINOR), glewGetString(GLEW_VERSION_MICRO));
			ImGui::BulletText("ImGui %s", ImGui::GetVersion());
			ImGui::BulletText("OpenGl %s", glGetString(GL_VERSION));
			ImGui::Spacing();
			ImGui::Spacing();

			// TODO: licencia
		}
		ImGui::End();
	}

	return UPDATE_CONTINUE;
}

update_status ModuleUI::PostUpdate(float dt)
{
	App->window->ResizeScreen();
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