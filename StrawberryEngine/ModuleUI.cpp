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

	isHierarchyActive = true;
	isInspectorActive = true;
	isConsoleActive = false;
	isAssetsActive = true;

	return ret;
}

update_status ModuleUI::PreUpdate(float dt)
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	/*SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
		if (event.type == SDL_QUIT)
			done = UPDATE_CONTINUE;
		if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(App->window->window))
			done = UPDATE_CONTINUE;
	}*/

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

update_status ModuleUI::Update(float dt)
{
	
	isAnyWindowHovered = false;
	
	ShowMenuBar();

	if(isDemoActive)
		ImGui::ShowDemoWindow();

	if (isConfigActive)
	{
		ShowConfig();
	}

	if (isHierarchyActive) 
	{
		ShowHierarchy();
	}

	if (isInspectorActive)
	{
		ShowInspector();
	}

	if (isAboutActive) 
	{
		ShowAbout();
	}

	if (isConsoleActive)
	{
		ShowConsole();
	}

	if (isAssetsActive)
	{
		ShowAssets();
	}

	ImGuiContext& g = *GImGui;
	if (g.DragDropActive && isDropTargetActive)
	{
		ShowDragTarget();
	}
	else
	{
		draggedMesh = nullptr;
		draggedTexture = nullptr;
		isDropTargetActive = false;
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

void ModuleUI::HandleSDLInput(SDL_Event* e)
{
	ImGui_ImplSDL2_ProcessEvent(e);
}

void ModuleUI::Draw() 
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ModuleUI::AddConsoleOutput(const char* text)
{
	pendingOutputs.push_back(strdup(text));
	isNewOutput = true;
}

void ModuleUI::UnselectAll()
{
	for (std::list<GameObject*>::iterator goIterator = App->scene_intro->gameObjectSelected.begin(); goIterator != App->scene_intro->gameObjectSelected.end(); goIterator++)
	{
		(*goIterator)->isSelected = false;
	}
	App->scene_intro->gameObjectSelected.clear();
}

void ModuleUI::ShowMenuBar()
{
	ImGui::BeginMainMenuBar();
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Quit"))
			{
				App->input->quit = true;
			}

			if (ImGui::MenuItem("Save"))
			{

			}

			if (ImGui::MenuItem("Load"))
			{

			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			ImGui::Checkbox("Demo", &isDemoActive);
			ImGui::Checkbox("Configuration", &isConfigActive);
			ImGui::Checkbox("Hierarchy", &isHierarchyActive);
			ImGui::Checkbox("Inspector", &isInspectorActive);
			ImGui::Checkbox("Console", &isConsoleActive);
			ImGui::Checkbox("Assets", &isAssetsActive);

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
				isAboutActive = !isAboutActive;

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
}

void ModuleUI::ShowConfig()
{
	ImGui::Begin("Configuration", &isConfigActive);
	{
		if (ImGui::IsWindowHovered())
		{
			isAnyWindowHovered = true;
		}
		if (ImGui::CollapsingHeader("Application"))
		{
			if (ImGui::InputText("Engine Name", (char*)name.c_str(), 32))
			{
				App->SetTitleName(name);
			}
			ImGui::InputText("Organization", (char*)organization.c_str(), 32);
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

void ModuleUI::ShowHierarchy()
{
	if (!isHierarchyInit)
	{
		isHierarchyInit = true;
		ImGui::SetNextWindowPos({ 10, 30 });
		ImGui::SetNextWindowSize({ 250, (float)(App->window->screen_surface->h - 40) });
	}

	ImGui::Begin("Hierarchy", &isHierarchyActive);
	if (ImGui::IsWindowHovered())
	{
		isAnyWindowHovered = true;
	}
	CreateHierarchy(App->scene_intro->rootNode);
	ImGui::End();
}

void ModuleUI::CreateHierarchy(GameObject* go)
{

	ImGuiTreeNodeFlags flags;
	if (go == App->scene_intro->rootNode)
	{
		flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
	}
	else if (go->children.size() > 0)
	{
		flags = ImGuiTreeNodeFlags_OpenOnArrow;
	}
	else
	{
		flags = ImGuiTreeNodeFlags_Leaf;
	}
	

	if (!go->isActive)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, { 0.5,0.5,0.5,0.5 });
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Text, { 1,1,1,1 });
	}


	if (go->isSelected)
	{
		flags |= ImGuiTreeNodeFlags_Selected;
	}


	if (ImGui::TreeNodeEx(go->name.c_str(), flags))
	{

		if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered())
		{
			UnselectAll();
		}

		if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1))
		{
			if (!go->isSelected)
			{
				UnselectAll();
				go->isSelected = true;
			}
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (ImGui::AcceptDragDropPayload("Drag go"))
			{
				draggedGameObject->parent->children.remove(draggedGameObject);
				go->AddChild(draggedGameObject);
				draggedGameObject->parent = go;
				draggedGameObject->isMoved = true;
				go->isMoved = true;
			}
			ImGui::EndDragDropTarget();
		}

		if (ImGui::BeginDragDropSource() && go != App->scene_intro->rootNode)
		{
			ImGui::SetDragDropPayload("Drag go", go, sizeof(GameObject));
			draggedGameObject = go;

			ImGui::EndDragDropSource();
		}

		if (ImGui::IsItemClicked(1))
		{
			isReparentingActive = true;
		}

		if (isReparentingActive && go->isSelected && go != App->scene_intro->rootNode)
		{

			//ImGui::SetNextWindowPos({ 200, 200 });
			if (ImGui::BeginPopupContextWindow())
			{
				if (ImGui::MenuItem("Add Child"))
				{
					App->scene_intro->AddEmptyGameObject(go);
				}
				if (ImGui::MenuItem("Remove GameObject (SUPR)"))
				{
					App->scene_intro->AddChildrenToDeathRow(go);
				}
				if (go->parent != App->scene_intro->rootNode)
				{
					if (ImGui::MenuItem("Reparent Up"))
					{
						App->scene_intro->gameObjectsToReparent.push_back(go);
					}
				}

				ImGui::EndPopup();
			}
		}


		for (std::list<GameObject*>::iterator childrenIterator = go->children.begin(); childrenIterator != go->children.end(); childrenIterator++)
		{
			CreateHierarchy(*childrenIterator);
		}

		ImGui::TreePop();
	}

	ImGui::PopStyleColor();

}

void ModuleUI::ShowInspector()
{
	if (!isInspectorInit)
	{
		isInspectorInit = true;
		ImGui::SetNextWindowPos({ (float)App->window->screen_surface->w - 410, 30 });
		ImGui::SetNextWindowSize({ 400, (float)(App->window->screen_surface->h - 40) });
	}
	ImGui::Begin("Inspector", &isInspectorActive);
	{
		if (ImGui::IsWindowHovered())
		{
			isAnyWindowHovered = true;
		}
		if (!App->scene_intro->gameObjectSelected.empty())
		{
			std::list<GameObject*>::iterator goIterator = App->scene_intro->gameObjectSelected.begin();
		
			if (ImGui::Checkbox("Active", &(*goIterator)->isActive))
			{
				(*goIterator)->SetActive((*goIterator)->isActive);
			}
			ImGui::SameLine();
			ImGui::InputText("", (char*)(*goIterator)->name.c_str(), 50);
			
			if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Spacing();
				// I'm gonna 'copy' Unity's
				ImGui::Text("Position"); ImGui::SameLine(); ImGui::Text("X"); ImGui::SameLine(); ImGui::PushItemWidth(75); ImGui::PushID("posX");
				if (ImGui::DragFloat("", &(*goIterator)->position.x))
				{
					(*goIterator)->isMoved = true;
				}
				ImGui::PopID();
				ImGui::SameLine(); ImGui::Text("Y"); ImGui::SameLine(); ImGui::PushItemWidth(75); ImGui::PushID("posY");
				if (ImGui::DragFloat("", &(*goIterator)->position.y))
				{
					(*goIterator)->isMoved = true;
				}
				ImGui::PopID();
				ImGui::SameLine(); ImGui::Text("Z"); ImGui::SameLine(); ImGui::PushItemWidth(75); ImGui::PushID("posZ");
				if (ImGui::DragFloat("", &(*goIterator)->position.z))
				{
					(*goIterator)->isMoved = true;
				}
				ImGui::PopID();


				////


				ImGui::Text("Rotation"); ImGui::SameLine(); ImGui::Text("X"); ImGui::SameLine(); ImGui::PushItemWidth(75); ImGui::PushID("rotX");
				if (ImGui::DragFloat("", &(*goIterator)->rotation.x))
				{
					(*goIterator)->isMoved = true;
				}
				ImGui::PopID();
				ImGui::SameLine(); ImGui::Text("Y"); ImGui::SameLine(); ImGui::PushItemWidth(75); ImGui::PushID("rotY");
				if (ImGui::DragFloat("", &(*goIterator)->rotation.y))
				{
					(*goIterator)->isMoved = true;
				}
				ImGui::PopID();
				ImGui::SameLine(); ImGui::Text("Z"); ImGui::SameLine(); ImGui::PushItemWidth(75); ImGui::PushID("rotZ");
				if (ImGui::DragFloat("", &(*goIterator)->rotation.z))
				{
					(*goIterator)->isMoved = true;
				}
				ImGui::PopID();


				////


				ImGui::Text("Scale   "); ImGui::SameLine(); ImGui::Text("X"); ImGui::SameLine(); ImGui::PushItemWidth(75); ImGui::PushID("sclX");
				if (ImGui::DragFloat("", &(*goIterator)->scale.x))
				{
					(*goIterator)->isMoved = true;
				}
				ImGui::PopID();
				ImGui::SameLine(); ImGui::Text("Y"); ImGui::SameLine(); ImGui::PushItemWidth(75); ImGui::PushID("sclY");
				if (ImGui::DragFloat("", &(*goIterator)->scale.y))
				{
					(*goIterator)->isMoved = true;
				}
				ImGui::PopID();
				ImGui::SameLine(); ImGui::Text("Z"); ImGui::SameLine(); ImGui::PushItemWidth(75); ImGui::PushID("sclZ");
				if (ImGui::DragFloat("", &(*goIterator)->scale.z))
				{
					(*goIterator)->isMoved = true;
				}
				ImGui::PopID();

			}

			if ((*goIterator)->meshComponent != nullptr)
			{
				ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
				if (ImGui::CollapsingHeader("Mesh Component", ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::PushID("MeshActive");
					ImGui::Checkbox("Active", &(*goIterator)->meshComponent->isDrawEnabled);
					ImGui::PopID();

					ImGui::SameLine();
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.5, 0, 0, 0.75 });
					ImGui::PushStyleColor(ImGuiCol_Button, { 0.85, 0, 0, 1 });
					if (ImGui::Button("Remove Mesh"))
					{
						App->scene_intro->meshComponentsToDelete.push_back(*goIterator);
					}
					ImGui::PopStyleColor();
					ImGui::PopStyleColor();


					if ((*goIterator)->meshComponent->path != nullptr)
					{
						ImGui::Spacing();
						ImGui::Text("Path:");
						ImGui::TextColored({ 1,0,1,1 }, (*goIterator)->meshComponent->path);
					}

					ImGui::Spacing();
					ImGui::Text("Num. of vertices:"); ImGui::SameLine(); ImGui::TextColored({ 1,0,1,1 }, std::to_string((*goIterator)->meshComponent->num_vertex).c_str());

					if ((*goIterator)->meshComponent->hasNormals)
					{
						ImGui::Spacing();
						ImGui::Checkbox("Draw Vertex Normals", &(*goIterator)->isVertexNormalsEnabled);
					}

					ImGui::Spacing();
					ImGui::Checkbox("Draw Bounding Boxes", &(*goIterator)->isAABBEnabled);

					ImGui::Spacing();
					if (ImGui::Button("Change Mesh", { 200, 40 }))
					{
						isChangeMeshActive = true;
					}

					if (isChangeMeshActive)
					{
						if (ImGui::BeginPopupContextWindow("Change mesh", ImGuiPopupFlags_MouseButtonLeft))
						{
							GameObject* go = (*App->scene_intro->gameObjectSelected.begin());

							for (std::list<MeshComponent*>::iterator meshIterator = App->scene_intro->meshesList.begin(); meshIterator != App->scene_intro->meshesList.end(); meshIterator++)
							{
								if ((*meshIterator) != go->meshComponent)
								{
									if (ImGui::MenuItem((*meshIterator)->name.c_str()))
									{
										go->meshComponent = (*meshIterator);
										go->isMoved = true;
										go->meshComponent->isDrawEnabled = true;
										isChangeMeshActive = false;
									}
								}
							}

							ImGui::EndPopup();
						}
					}
				}
			}
			else // Give option to add one
			{
				ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

				if (ImGui::Button("Add Mesh", { 200, 40 }))
				{
					isAddMeshActive = true;
				}

				if (isAddMeshActive)
				{
					//ImGuiPopupFlags_
					if (ImGui::BeginPopupContextWindow("Add mesh", ImGuiPopupFlags_MouseButtonLeft))
					{
						for (std::list<MeshComponent*>::iterator meshIterator = App->scene_intro->meshesList.begin(); meshIterator != App->scene_intro->meshesList.end(); meshIterator++)
						{
							if (ImGui::MenuItem((*meshIterator)->name.c_str()))
							{
								GameObject* go = (*App->scene_intro->gameObjectSelected.begin());
								go->meshComponent = (*meshIterator);
								go->isMoved = true;
								go->meshComponent->isDrawEnabled = true;
								isAddMeshActive = false;
							}
						}

						ImGui::EndPopup();
					}
				}
			}




			goIterator = App->scene_intro->gameObjectSelected.begin();
			if ((*goIterator)->textureComponent != nullptr)
			{
				ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
				if (ImGui::CollapsingHeader("Texture Component", ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::PushID("TexActive");
					ImGui::Checkbox("Active", &(*goIterator)->textureComponent->isActive);
					ImGui::PopID();

					ImGui::SameLine();
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.5, 0, 0, 0.75 });
					ImGui::PushStyleColor(ImGuiCol_Button, { 0.85, 0, 0, 1 });
					if (ImGui::Button("Remove Texture"))
					{
						App->scene_intro->textureComponentsToDelete.push_back(*goIterator);
					}
					ImGui::PopStyleColor();
					ImGui::PopStyleColor();

					if ((*goIterator)->textureComponent->texPath != nullptr && (*goIterator)->textureComponent->texPath != "")
					{
						ImGui::Spacing();
						ImGui::Text("Path:");
						ImGui::TextColored({ 1,0,1,1 }, (*goIterator)->textureComponent->texPath);
					}

					ImGui::Spacing();
					ImGui::Text("Width:"); ImGui::SameLine(); ImGui::TextColored({ 1,0,1,1 }, std::to_string((*goIterator)->textureComponent->w).c_str());
					ImGui::Text("Height:"); ImGui::SameLine(); ImGui::TextColored({ 1,0,1,1 }, std::to_string((*goIterator)->textureComponent->h).c_str());
					ImGui::Image((ImTextureID)(*goIterator)->textureComponent->GetId(), { 150,150 });

					ImGui::Spacing();
					if (ImGui::Button("Change Texture", { 200, 40 }))
					{
						isChangeTexActive = true;
					}

					if (isChangeTexActive)
					{
						//ImGuiPopupFlags_
						if (ImGui::BeginPopupContextWindow("Change tex", ImGuiPopupFlags_MouseButtonLeft))
						{
							GameObject* go = (*App->scene_intro->gameObjectSelected.begin());
							for (std::list<TextureComponent*>::iterator textureIterator = App->scene_intro->textureList.begin(); textureIterator != App->scene_intro->textureList.end(); textureIterator++)
							{
								if ((*textureIterator) != go->textureComponent)
								{
									if (ImGui::MenuItem((*textureIterator)->name))
									{
										go->textureComponent = (*textureIterator);
										go->textureComponent->isActive = true;
										isChangeTexActive = false;
									}
								}
							}

							ImGui::EndPopup();
						}
					}
				}
			}
			else
			{
				ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

				if (ImGui::Button("Add Texture", { 200, 40 }))
				{
					isAddTexActive = true;
				}

				if (isAddTexActive)
				{
					//ImGuiPopupFlags_
					if (ImGui::BeginPopupContextWindow("Add tex", ImGuiPopupFlags_MouseButtonLeft))
					{
						for (std::list<TextureComponent*>::iterator textureIterator = App->scene_intro->textureList.begin(); textureIterator != App->scene_intro->textureList.end(); textureIterator++)
						{
							if (ImGui::MenuItem((*textureIterator)->name))
							{
								GameObject* go = (*App->scene_intro->gameObjectSelected.begin());
								go->textureComponent = (*textureIterator);
								go->textureComponent->isActive = true;
								isAddTexActive = false;
							}
						}

						ImGui::EndPopup();
					}
				}
			}
		}
	}

	ImGui::End();
}

void ModuleUI::ShowAbout()
{
	ImGui::Begin("About", &isAboutActive);
	{
		if (ImGui::IsWindowHovered())
		{
			isAnyWindowHovered = true;
		}
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

void ModuleUI::ShowConsole()
{
	if (!isConsoleInit)
	{
		isConsoleInit = true;
		ImGui::SetNextWindowPos({ (float)App->window->screen_surface->w - 410, 230 });
		ImGui::SetNextWindowSize({ 400, (float)(App->window->screen_surface->h - 240) });
	}

	ImGui::Begin("Console", &isConsoleActive, ImGuiWindowFlags_HorizontalScrollbar);

	if (ImGui::IsWindowHovered())
	{
		isAnyWindowHovered = true;
	}

	for (std::list<const char*>::iterator consoleOutputs = pendingOutputs.begin(); consoleOutputs != pendingOutputs.end(); consoleOutputs++)
	{
		ImGui::Text((*consoleOutputs));
		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
	}

	if (isNewOutput)
	{
		ImGui::SetScrollHereY(1.0f);
		isNewOutput = false;
	}
	ImGui::End();
}

void ModuleUI::ShowAssets()
{
	if (!isAssetsInit)
	{
		isAssetsInit = true;
		ImGui::SetNextWindowPos({ 270, (float)(App->window->screen_surface->h - 210) });
		ImGui::SetNextWindowSize({ (float)(App->window->screen_surface->w - 690), 200 });
	}
	ImGui::Begin("Assets", &isAssetsActive);

	if (ImGui::IsWindowHovered())
	{
		isAnyWindowHovered = true;
	}

	if (ImGui::TreeNodeEx("Assets"))
	{
		if (ImGui::TreeNodeEx("Textures"))
		{
			for (std::list<TextureComponent*>::iterator textureIterator = App->scene_intro->textureList.begin(); textureIterator != App->scene_intro->textureList.end(); textureIterator++)
			{
				if (ImGui::TreeNodeEx((*textureIterator)->name, ImGuiTreeNodeFlags_Leaf))
				{
					if (ImGui::BeginDragDropSource())
					{
						ImGui::SetDragDropPayload("Drag tex", (*textureIterator), sizeof(TextureComponent));
						draggedTexture = (*textureIterator);
						isDropTargetActive = true;
						ImGui::EndDragDropSource();
					}

					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNodeEx("Meshes"))
		{
			for (std::list<MeshComponent*>::iterator meshIterator = App->scene_intro->meshesList.begin(); meshIterator != App->scene_intro->meshesList.end(); meshIterator++)
			{
				if (ImGui::TreeNodeEx((*meshIterator)->name.c_str(), ImGuiTreeNodeFlags_Leaf))
				{
					if (ImGui::BeginDragDropSource())
					{
						ImGui::SetDragDropPayload("Drag mesh", (*meshIterator), sizeof(MeshComponent));
						draggedMesh = (*meshIterator);
						isDropTargetActive = true;
						ImGui::EndDragDropSource();
					}

					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}



		ImGui::TreePop();
	}

	ImGui::End();
}

void ModuleUI::ShowDragTarget()
{
	ImGui::SetNextWindowSize({ 300, 300 });
	ImGui::SetNextWindowPos({ 400,  200 });
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar;

	ImGui::Begin("a", 0, flags);

	if (ImGui::IsWindowHovered())
	{
		isAnyWindowHovered = true;
	}

	ImGui::Text("\n\n\n\n\n\n\n\n\n\n                  Drop                   \n                  Here                   \n\n\n\n\n\n\n\n\n\n\n");

	if (ImGui::BeginDragDropTarget())
	{
		if (ImGui::AcceptDragDropPayload("Drag tex"))
		{
			if (!App->scene_intro->gameObjectSelected.empty())
			{
				GameObject* go = (*App->scene_intro->gameObjectSelected.begin());
				go->textureComponent = draggedTexture;
				go->textureComponent->isActive = true;
			}
		}

		else if (ImGui::AcceptDragDropPayload("Drag mesh"))
		{
			if (!App->scene_intro->gameObjectSelected.empty())
			{
				GameObject* go = (*App->scene_intro->gameObjectSelected.begin());
				go->meshComponent = draggedMesh;
				go->meshComponent->isDrawEnabled = true;
				go->isMoved = true;
			}
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::End();
}

void ModuleUI::DrawFrame(GLuint frameBuffer)
{
	//ImGui::SetNextWindowSize({ 800, 600 });
	ImGui::Begin("Scene");
	ImGui::Image((ImTextureID)App->renderer3D->texColorBuffer, ImVec2( App->window->screen_surface->w,App->window->screen_surface->h ), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
}

const bool ModuleUI::IsAnyWindowHovered() const
{
	return isAnyWindowHovered;
}
