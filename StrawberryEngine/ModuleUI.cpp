#include "Globals.h"
#include "Application.h"
#include "ModuleUI.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleImporter.h"
#include "GameObject.h"

#include "Libs/DevIL/include/IL/il.h"
#include "Libs/DevIL/include/IL/ilu.h"
#include "Libs/DevIL/include/IL/ilut.h"

#include "Libs/Assimp/include/version.h"


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
	
	isTexturesEnabled = true;
	isDrawEnabled = true;

	isHierarchyShown = true;
	isInspectorShown = true;

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
			ImGui::Checkbox("Inspector", &isInspectorShown);

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{

			if (ImGui::MenuItem("Documentation")) {}
				//App->RequestBrowser("https://needlesslord.github.io/StrawberryEngine/");

			if (ImGui::MenuItem("Download latest release")) {}
				//App->RequestBrowser("https://github.com/Needlesslord/StrawberryEngine/releases/tag/v0.2");

			if (ImGui::MenuItem("Current release")) {}
				//App->RequestBrowser("https://github.com/Needlesslord/StrawberryEngine/releases/tag/v0.2");

			if (ImGui::MenuItem("All the releases")) {}
				//App->RequestBrowser("https://github.com/Needlesslord/StrawberryEngine/releases");

			if (ImGui::MenuItem("Report a bug")) {}
				//App->RequestBrowser("https://github.com/Needlesslord/StrawberryEngine/issues");

			if (ImGui::MenuItem("About"))
				isAboutShown = !isAboutShown;

			ImGui::EndMenu();
		}
	}
	ImGui::EndMainMenuBar();

	if(isDemoShown)
		ImGui::ShowDemoWindow();

	if (isConfigShown)
	{
		
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
				ImGui::TextColored({ 1,0,1,1 }, "2.0.12"); ImGui::Separator();
				ImGui::Text("CPU's:"); ImGui::SameLine();
				ImGui::TextColored({ 1,0,1,1 }, "%d (Cache: %dKb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());
				ImGui::Text("System RAM:"); ImGui::SameLine();
				ImGui::TextColored({ 1,0,1,1 }, "%dGb", SDL_GetSystemRAM() / 1000);
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
				if (SDL_HasSSE42())ImGui::TextColored({ 1,0,1,1 }, "SSE42,"); ImGui::Separator();
			}



			if (ImGui::CollapsingHeader("Render"))
			{
				
				char* a = "on";
				char* b = "off";
				if (ImGui::Checkbox("Draw Meshes", &isDrawEnabled))
				{
					LOG("Turning rendering: %s", isDrawEnabled ? a : b);
				}
				if (ImGui::Checkbox("Show depth test", &isDepthTestEnabled))
				{
					App->renderer3D->ToggleDepthTest(isDepthTestEnabled);
					LOG("Turning depth test: %s", isDepthTestEnabled ? a : b);
				}
				if (ImGui::Checkbox("Show lighting", &isLightingEnabled))
				{
					App->renderer3D->ToggleLighting(isLightingEnabled);
					LOG("Turning lighting: %s", isLightingEnabled ? a : b);
				}
				if (ImGui::Checkbox("Show back face cull", &isBackFaceCullEnabled))
				{
					App->renderer3D->ToggleBackFaceCull(isBackFaceCullEnabled);
					LOG("Turning back face cull: %s", isBackFaceCullEnabled ? a : b);
				}
				
				if (ImGui::Checkbox("Show wireframes", &isWireframeEnabled))
				{
					App->renderer3D->ToggleWireframe(isWireframeEnabled);
					LOG("Turning wireframes: %s", isWireframeEnabled ? a : b);
				}
			}



			if (ImGui::CollapsingHeader("Textures"))
			{
				char* a = "on";
				char* b = "off";
				if (ImGui::Checkbox("Show textures", &isTexturesEnabled))
				{
					App->renderer3D->ToggleTextures(&isTexturesEnabled);
					LOG("Turning textures: %s", isTexturesEnabled ? a : b);
				}
			}
		}
		ImGui::End();
	}




	if (isHierarchyShown) //  ImGui::IsWindowHovered() !!!
	{
		/*ImGui::SetNextWindowPos({ 20, 20 });
		ImGui::SetNextWindowSize({ 300, (float)(App->window->screen_surface->h * 8 / 10) });*/
		ImGui::Begin("Hierarchy");

		if (App->scene_intro->gameObjectList.size() > 0) {

			for (std::list<GameObject*>::iterator gameObjectIterator = App->scene_intro->gameObjectList.begin(); gameObjectIterator != App->scene_intro->gameObjectList.end(); gameObjectIterator++)
			{
				//ImGui::Checkbox((*gameObjectIterator)->name, &(*gameObjectIterator)->selected);

				if (ImGui::TreeNode((*gameObjectIterator)->name))
				{

					if (!(*gameObjectIterator)->childrenMeshes.empty())
					{

						for (std::list<Mesh*>::iterator meshIterator = (*gameObjectIterator)->childrenMeshes.begin(); meshIterator != (*gameObjectIterator)->childrenMeshes.end(); meshIterator++)
						{
							ImGui::Checkbox((*meshIterator)->name, &(*meshIterator)->isSelected);
						}
					}
					ImGui::TreePop();
				}
			}
		}

		ImGui::End();
	}




	if (isInspectorShown)
	{
		/*ImGui::SetNextWindowPos({ (float)App->window->screen_surface->w - 370, 20 });
		ImGui::SetNextWindowSize({ 350, (float)(App->window->screen_surface->h * 8 / 10) });*/

		ImGui::Begin("Inspector", &isInspectorShown);
		{

			if (!App->scene_intro->meshesSelected.empty())
			{
				std::list<Mesh*>::iterator meshIterator = App->scene_intro->meshesSelected.begin();
				if (ImGui::CollapsingHeader("Transformation"))
				{
					float v1[3] = { (*meshIterator)->position.x, (*meshIterator)->position.y, (*meshIterator)->position.z };
					ImGui::InputFloat3("Position", v1, 2);

					float v2[3] = { (*meshIterator)->rotation.x, (*meshIterator)->rotation.y, (*meshIterator)->rotation.z };
					ImGui::InputFloat3("Rotation", v2, 2);

					float v3[3] = { (*meshIterator)->scale.x, (*meshIterator)->scale.y, (*meshIterator)->scale.z };
					ImGui::InputFloat3("Scale", v3, 2);
				}

				if (ImGui::CollapsingHeader("Render"))
				{
					if (ImGui::Checkbox("Draw Meshes", &(*meshIterator)->isDrawEnabled))
					{
						char* a = "on";
						char* b = "off";
						bool helper = (*meshIterator)->isDrawEnabled;
						for (meshIterator = App->scene_intro->meshesSelected.begin(); meshIterator != App->scene_intro->meshesSelected.end(); meshIterator++)
						{
							if (helper)
							{
								(*meshIterator)->isDrawEnabled = true;
							}
							else
							{
								(*meshIterator)->isDrawEnabled = false;
							}

							LOG("Turning rendering: %s for %s", (*meshIterator)->isDrawEnabled ? a : b, (*meshIterator)->name);
						}
					}


					for (meshIterator = App->scene_intro->meshesSelected.begin(); meshIterator != App->scene_intro->meshesSelected.end(); meshIterator++)
					{
						ImGui::Text("Mesh path for %s:", (*meshIterator)->name); 
						ImGui::TextColored({ 1,0,1,1 }, (*meshIterator)->path);
						ImGui::Separator();
					}
				}


				if (ImGui::CollapsingHeader("Textures"))
				{

					std::list<Texture*>::iterator textureIterator = App->importer->textureList.begin();

					//ImGui::BeginCombo("Textures", (*textureIterator)->name); // LOOK UP

					int i = 0;
					while (textureIterator != App->importer->textureList.end())
					{
						if (ImGui::Button((*textureIterator)->name))
						{
							for (std::list<Mesh*>::iterator meshesToChangeTexture = App->scene_intro->meshesSelected.begin(); meshesToChangeTexture != App->scene_intro->meshesSelected.end(); meshesToChangeTexture++)
							{
								(*meshesToChangeTexture)->textureNumber = i;
							}
						}
						i++;
						textureIterator++;

					}



					if (ImGui::Button("No texture"))
					{
						for (std::list<Mesh*>::iterator meshesToChangeTexture = App->scene_intro->meshesSelected.begin(); meshesToChangeTexture != App->scene_intro->meshesSelected.end(); meshesToChangeTexture++)
						{
							(*meshesToChangeTexture)->textureNumber = 999;
						}
					}



					for (textureIterator = App->importer->textureList.begin(); textureIterator != App->importer->textureList.end(); textureIterator++)
					{
						if ((*textureIterator)->name != nullptr && (*textureIterator)->path != nullptr)
						{
							ImGui::Text("Texture path for %s:", (*textureIterator)->name);
							ImGui::TextColored({ 1,0,1,1 }, (*textureIterator)->path);
							ImGui::Separator();
						}
					}
				}
			}
		}

		ImGui::End();
	}





	if (isAboutShown) 
	{
		ImGui::Begin("About", &isAboutShown);
		{
			ImGui::Text("Strawberry Engine");
			ImGui::Text("The sweetest 3D Engine");
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Text("By Tomas Carreras and Nuria Lamonja. Supervised by professor Marc Garrigo");
			ImGui::Spacing();
			ImGui::Spacing();
			if (ImGui::MenuItem("GitHub Web Page"))
				App->RequestBrowser("https://github.com/Needlesslord/StrawberryEngine");
			ImGui::Spacing();
			ImGui::Text("3rd Party Libraries used:");

			SDL_version version;
			SDL_VERSION(&version);
			ImGui::BulletText("SDL %d.%d.%d", version.major, version.minor, version.patch);
			ImGui::BulletText("Glew %s.%s.%s", glewGetString(GLEW_VERSION_MAJOR), glewGetString(GLEW_VERSION_MINOR), glewGetString(GLEW_VERSION_MICRO));
			ImGui::BulletText("ImGui %s", ImGui::GetVersion());
			ImGui::BulletText("OpenGl %s", glGetString(GL_VERSION));
			ImGui::BulletText("DevIL %d", IL_VERSION);
			ImGui::BulletText("Assimp %d.%d.%d", aiGetVersionMajor(), aiGetVersionMinor(), aiGetVersionRevision());



			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::Text("MIT License");
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Text("     Copyright(c) 2020 Nuria Lamonja @needlesslord + Tomas Carreras @tomascarreras1000");
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Text("     Permission is hereby granted, free of charge, to any person obtaining a copy");
			ImGui::Text("        of this softwareand associated documentation files(the 'Software'), to deal");
			ImGui::Text("        in the Software without restriction, including without limitation the rights");
			ImGui::Text("        to use, copy, modify, merge, publish, distribute, sublicense, and /or sell");
			ImGui::Text("        copies of the Software, and to permit persons to whom the Software is");
			ImGui::Text("        furnished to do so, subject to the following conditions :");
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Text("     The above copyright noticeand this permission notice shall be included in all");
			ImGui::Text("        copies or substantial portions of the Software.");
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Text("     THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR");
			ImGui::Text("        IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,");
			ImGui::Text("        FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE");
			ImGui::Text("        AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER");
			ImGui::Text("        LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,");
			ImGui::Text("        OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE");
			ImGui::Text("        SOFTWARE.");
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