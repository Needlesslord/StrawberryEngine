#ifndef __ModuleUI_H__
#define __ModuleUI_H__

#include "Module.h"
#include "Libs/ImGUI/imgui.h"

class Application;

class ModuleUI : public Module
{
public:

	ModuleUI(Application* app, bool start_enabled = true);

	bool Start();

	update_status PreUpdate(float dt);

	update_status Update(float dt);

	update_status PostUpdate(float dt);

	bool CleanUp();


	// Destructor
	virtual ~ModuleUI();

public:

	bool done = false;

	bool show_demo_window = true;

	bool show_another_window = false;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

};

#endif // __ModuleUI_H__