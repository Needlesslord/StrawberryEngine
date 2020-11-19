#include "Globals.h"
#include "Application.h"
#include "ModuleUI.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleImporter.h"
#include "TextureImporter.h"
#include "MeshImporter.h"
#include "GameObject.h"

#include "Libs/DevIL/include/IL/il.h"
#include "Libs/DevIL/include/IL/ilu.h"
#include "Libs/DevIL/include/IL/ilut.h"

#include "Libs/Assimp/include/version.h"

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
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init("#version 130");
	LOG("Initializing ImGui");

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
			ImGui::Checkbox("Configuration", &isConfigShown);
			ImGui::Checkbox("Hierarchy", &isHierarchyShown);
			ImGui::Checkbox("Inspector", &isInspectorShown);

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{

			if (ImGui::MenuItem("Documentation"))
				App->RequestBrowser("https://needlesslord.github.io/StrawberryEngine/");

			if (ImGui::MenuItem("Download latest release"))
				App->RequestBrowser("https://github.com/Needlesslord/StrawberryEngine/releases/tag/v0.2");

			if (ImGui::MenuItem("Current release"))
				App->RequestBrowser("https://github.com/Needlesslord/StrawberryEngine/releases/tag/v0.2");

			if (ImGui::MenuItem("All the releases"))
				App->RequestBrowser("https://github.com/Needlesslord/StrawberryEngine/releases");

			if (ImGui::MenuItem("Report a bug"))
				App->RequestBrowser("https://github.com/Needlesslord/StrawberryEngine/issues");

			if (ImGui::MenuItem("About"))
				isAboutShown = !isAboutShown;

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Basic shapes"))
		{
			if (ImGui::MenuItem("Cube"))
			{
				App->importer->meshImporter->LoadMesh("Assets/Primitives/Cube.FBX");
				App->renderer3D->GenerateBuffers();
			}

			if (ImGui::MenuItem("Cylinder"))
			{
				App->importer->meshImporter->LoadMesh("Assets/Primitives/Cylinder.FBX");
				App->renderer3D->GenerateBuffers();
			}

			if (ImGui::MenuItem("Piramid"))
			{
				App->importer->meshImporter->LoadMesh("Assets/Primitives/Piramid.FBX");
				App->renderer3D->GenerateBuffers();
			}

			if (ImGui::MenuItem("Sphere"))
			{
				App->importer->meshImporter->LoadMesh("Assets/Primitives/Sphere.FBX");
				App->renderer3D->GenerateBuffers();
			}

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
		if (!isHierarchyInit)
		{
			isHierarchyInit = true;
			ImGui::SetNextWindowPos({ 10, 30 });
			ImGui::SetNextWindowSize({ 250, (float)(App->window->screen_surface->h - 40) });
		}
		/*ImGui::SetNextWindowPos({ 20, 20 });
		ImGui::SetNextWindowSize({ 300, (float)(App->window->screen_surface->h * 8 / 10) });*/
		ImGui::Begin("Hierarchy", &isHierarchyShown);

		if (App->scene_intro->gameObjectList.size() > 0) 
		{

			for (std::list<GameObject*>::iterator gameObjectIterator = App->scene_intro->gameObjectList.begin(); gameObjectIterator != App->scene_intro->gameObjectList.end(); gameObjectIterator++)
			{
				//ImGui::Checkbox((*gameObjectIterator)->name, &(*gameObjectIterator)->selected);

				if (ImGui::TreeNode((*gameObjectIterator)->name))
				{

					for (std::list<GameObject*>::iterator childrenIterator = (*gameObjectIterator)->children.begin(); childrenIterator != (*gameObjectIterator)->children.end(); childrenIterator++)
					{
						if (ImGui::Checkbox((*childrenIterator)->name, &(*childrenIterator)->isSelected))
						{
							if (App->input->GetKey(SDL_SCANCODE_LCTRL) != KEY_REPEAT && (*childrenIterator)->isSelected)
							{
								UnselectAll();
							}
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
		if (!isInspectorInit)
		{
			isInspectorInit = true;
			ImGui::SetNextWindowPos({ (float)App->window->screen_surface->w - 260, 30 });
			ImGui::SetNextWindowSize({ 250, (float)(App->window->screen_surface->h -  40)});
		}
		ImGui::Begin("Inspector", &isInspectorShown);
		{

			if (!App->scene_intro->gameObjectSelected.empty())
			{
				std::list<GameObject*>::iterator goIterator = App->scene_intro->gameObjectSelected.begin();

				char* buf = (*goIterator)->name;

				if (ImGui::InputText("", buf, 50, ImGuiInputTextFlags_EnterReturnsTrue))
					(*goIterator)->name = buf;

				if (ImGui::CollapsingHeader("Transformation", ImGuiTreeNodeFlags_DefaultOpen))
				{
					//float v1[3] = { (*meshIterator)->position.x, (*meshIterator)->position.y, (*meshIterator)->position.z };
					//ImGui::InputFloat3("Position", v1);

					if (ImGui::DragFloat3("Position", &(*goIterator)->position))
					{
						//float v1[3] = { (*meshIterator)->position.x, (*meshIterator)->position.y, (*meshIterator)->position.z };
						(*goIterator)->isMoved = true;
					}

					if (ImGui::DragFloat3("Rotation", &(*goIterator)->rotation))
					{
						//float v2[3] = { (*meshIterator)->rotation.x, (*meshIterator)->rotation.y, (*meshIterator)->rotation.z };
					}

					if (ImGui::DragFloat3("Scale", &(*goIterator)->scale))
					{
						//float v3[3] = { (*meshIterator)->scale.x, (*meshIterator)->scale.y, (*meshIterator)->scale.z };
					}
				}

				if (ImGui::CollapsingHeader("Render", ImGuiTreeNodeFlags_DefaultOpen))
				{
					if (ImGui::Checkbox("Draw Meshes", &(*goIterator)->meshComponent->isDrawEnabled))
					{
						char* a = "on";
						char* b = "off";
						bool helper = (*goIterator)->meshComponent->isDrawEnabled;
						for (goIterator = App->scene_intro->gameObjectSelected.begin(); goIterator != App->scene_intro->gameObjectSelected.end(); goIterator++)
						{
							if (helper)
							{
								(*goIterator)->meshComponent->isDrawEnabled = true;
							}
							else
							{
								(*goIterator)->meshComponent->isDrawEnabled = false;
							}

							LOG("Turning rendering: %s for %s", (*goIterator)->meshComponent->isDrawEnabled ? a : b, (*goIterator)->name);
						}
					}


					for (goIterator = App->scene_intro->gameObjectSelected.begin(); goIterator != App->scene_intro->gameObjectSelected.end(); goIterator++)
					{
						if ((*goIterator)->meshComponent->path != nullptr)
						{
							ImGui::Text("Mesh path for %s:", (*goIterator)->name);
							ImGui::TextColored({ 1,0,1,1 }, (*goIterator)->meshComponent->path);
							ImGui::Separator();
						}
					}

					if (ImGui::Checkbox("Draw Vertex Normals", &isVertexNormalsEnabled))
					{
						goIterator = App->scene_intro->gameObjectSelected.begin();
						char* a = "on";
						char* b = "off";
						
						for (goIterator = App->scene_intro->gameObjectSelected.begin(); goIterator != App->scene_intro->gameObjectSelected.end(); goIterator++)
						{
							if (isVertexNormalsEnabled)
							{
								(*goIterator)->isVertexNormalsEnabled = true;
							}
							else
							{
								(*goIterator)->isVertexNormalsEnabled = false;
							}

							LOG("Turning normals: %s for %s", (*goIterator)->isVertexNormalsEnabled ? a : b, (*goIterator)->name);
						}
					}
				}


				if (ImGui::CollapsingHeader("Textures", ImGuiTreeNodeFlags_DefaultOpen))
				{

					std::list<Texture*>::iterator textureIterator = App->scene_intro->textureList.begin();

					//ImGui::BeginCombo("Textures", (*textureIterator)->name); // LOOK UP

					int i = 0;
					while (textureIterator != App->scene_intro->textureList.end())
					{
						if (ImGui::Button((*textureIterator)->name))
						{
							for (std::list<GameObject*>::iterator goToChangeTexture = App->scene_intro->gameObjectSelected.begin(); goToChangeTexture != App->scene_intro->gameObjectSelected.end(); goToChangeTexture++)
							{
								(*goToChangeTexture)->meshComponent->textureNumber = i;
							}
						}
						i++;
						textureIterator++;
					}



					if (ImGui::Button("No texture"))
					{
						for (std::list<GameObject*>::iterator goToChangeTexture = App->scene_intro->gameObjectSelected.begin(); goToChangeTexture != App->scene_intro->gameObjectSelected.end(); goToChangeTexture++)
						{
							(*goToChangeTexture)->meshComponent->textureNumber = 999;
						}
					}



					for (textureIterator = App->scene_intro->textureList.begin(); textureIterator != App->scene_intro->textureList.end(); textureIterator++)
					{
						if ((*textureIterator)->name != nullptr && (*textureIterator)->path != nullptr)
						{
							ImGui::Separator();
							ImGui::Text("%s", (*textureIterator)->name);
							ImGui::Text("Texture size: %d * %d", (*textureIterator)->w, (*textureIterator)->h);
							ImGui::Text("Texture path:");
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
		}
		ImGui::End();
	}



	
	if (isConsoleShown)
	{
		
		if(!isConsoleInit)
		{
			isConsoleInit = true;
			ImGui::SetNextWindowPos({ 270, (float)(App->window->screen_surface->h - 210) });
			ImGui::SetNextWindowSize({ (float)(App->window->screen_surface->w - 540), 200 });
		}

		ImGui::Begin("Console", &isConsoleShown);

		for (std::list<const char*>::iterator consoleOutputs = pendingOutputs.begin(); consoleOutputs != pendingOutputs.end(); consoleOutputs++)
		{
			ImGui::Text((*consoleOutputs));
			ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
		}

		ImGui::SetScrollHereY(1.0f);
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

void ModuleUI::AddConsoleOutput(const char* text, ...)
{
	pendingOutputs.push_back(strdup(text));
}

void ModuleUI::UnselectAll()
{
	for (std::list<GameObject*>::iterator goIterator = App->scene_intro->gameObjectSelected.begin(); goIterator != App->scene_intro->gameObjectSelected.end(); goIterator++)
	{
		(*goIterator)->isSelected = false;
	}
	App->scene_intro->gameObjectSelected.clear();
}